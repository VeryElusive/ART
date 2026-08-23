#include "threading_platform.hpp"

#include "../common/memory.hpp"

#include <errno.h>
#include <limits.h>
#include <linux/futex.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#if defined(__i386__) || defined(__x86_64__)
#include <immintrin.h>
#endif

namespace
{
	using ThreadEntry_t = ART::Threading::Platform::ThreadEntry_t;

	struct ThreadStartContext
	{
		ThreadEntry_t Entry;
		void *Context;
	};

	struct SemaphoreStorage
	{
		sem_t Semaphore;
		i32 MaximumCount;
	};

	volatile i32 ActiveProcessorCount = 0;

	static_assert(sizeof(pthread_t) <= sizeof(Ptr_t), "thread storage is too small");
	static_assert(sizeof(ThreadStartContext) <= 32, "thread start storage is too small");
	static_assert(alignof(ThreadStartContext) <= 16, "thread start storage is misaligned");
	static_assert(sizeof(SemaphoreStorage) <= 64, "semaphore storage is too small");
	static_assert(alignof(SemaphoreStorage) <= 16, "semaphore storage is misaligned");

	void *LinuxThreadEntry(void *Parameter)
	{
		auto StartContext = (ThreadStartContext *)Parameter;
		StartContext->Entry(StartContext->Context);
		return NULL;
	}

	Ptr_t EncodeThreadID(const pthread_t Thread)
	{
		Ptr_t Result = 0;
		ART::Memcpy(&Result, &Thread, sizeof(Thread));
		return Result;
	}
}

bool ART::Threading::Platform::Initialize()
{
	return true;
}

Size_t ART::Threading::Platform::GetProcessorCount()
{
	const auto CachedCount = AtomicCompareExchange(&ActiveProcessorCount, 0, 0);
	if(CachedCount != 0)
	{
		return (Size_t)CachedCount;
	}

	const auto SystemCount = sysconf(_SC_NPROCESSORS_ONLN);
	const auto ValidCount = SystemCount <= 0 ? 1 : (i32)SystemCount;
	AtomicCompareExchange(&ActiveProcessorCount, ValidCount, 0);
	return (Size_t)AtomicCompareExchange(&ActiveProcessorCount, 0, 0);
}

i32 ART::Threading::Platform::AtomicCompareExchange(
	volatile i32 *Address, const i32 Exchange, const i32 Comparand
)
{
	i32 Expected = Comparand;
	__atomic_compare_exchange_n(
		Address, &Expected, Exchange, false,
		__ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST
	);
	return Expected;
}

i32 ART::Threading::Platform::AtomicExchange(
	volatile i32 *Address, const i32 Value
)
{
	return __atomic_exchange_n(Address, Value, __ATOMIC_SEQ_CST);
}

i32 ART::Threading::Platform::AtomicIncrement(volatile i32 *Address)
{
	return __atomic_add_fetch(Address, 1, __ATOMIC_SEQ_CST);
}

i32 ART::Threading::Platform::AtomicDecrement(volatile i32 *Address)
{
	return __atomic_sub_fetch(Address, 1, __ATOMIC_SEQ_CST);
}

void ART::Threading::Platform::Pause()
{
#if defined(__i386__) || defined(__x86_64__)
	_mm_pause();
#elif defined(__aarch64__)
	__asm__ __volatile__("yield");
#else
	__asm__ __volatile__("" ::: "memory");
#endif
}

bool ART::Threading::Platform::CreateThread(
	Ptr_t *Thread, void *StartContextStorage,
	const ThreadEntry_t Entry, void *Context, Ptr_t *ThreadID
)
{
	if(Thread == NULL || StartContextStorage == NULL || Entry == NULL)
	{
		return false;
	}

	auto StartContext = (ThreadStartContext *)StartContextStorage;
	StartContext->Entry = Entry;
	StartContext->Context = Context;

	pthread_t NativeThread;
	if(pthread_create(
		&NativeThread, NULL, LinuxThreadEntry, StartContext
	) != 0)
	{
		return false;
	}

	*Thread = EncodeThreadID(NativeThread);
	if(ThreadID != NULL)
	{
		*ThreadID = *Thread;
	}
	return true;
}

void ART::Threading::Platform::JoinThread(const Ptr_t Thread)
{
	pthread_t NativeThread;
	ART::Memcpy(&NativeThread, &Thread, sizeof(NativeThread));
	pthread_join(NativeThread, NULL);
}

void ART::Threading::Platform::DetachThread(const Ptr_t Thread)
{
	pthread_t NativeThread;
	ART::Memcpy(&NativeThread, &Thread, sizeof(NativeThread));
	pthread_detach(NativeThread);
}

Ptr_t ART::Threading::Platform::GetCurrentThreadID()
{
	return EncodeThreadID(pthread_self());
}

void ART::Threading::Platform::YieldThread()
{
	sched_yield();
}

void *ART::Threading::Platform::AllocateThreadMemory(const Size_t Size)
{
	if(Size == 0)
	{
		return NULL;
	}
	const auto Address = mmap(
		NULL, Size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0
	);
	return Address == MAP_FAILED ? NULL : Address;
}

void ART::Threading::Platform::FreeThreadMemory(
	void *Address, const Size_t Size
)
{
	if(Address != NULL && Size != 0)
	{
		munmap(Address, Size);
	}
}

bool ART::Threading::Platform::InitializeSemaphore(
	void *Storage, const i32 InitialCount, const i32 MaximumCount
)
{
	if(Storage == NULL || InitialCount < 0 || MaximumCount <= 0
		|| InitialCount > MaximumCount)
	{
		return false;
	}

	auto Semaphore = (SemaphoreStorage *)Storage;
	if(sem_init(&Semaphore->Semaphore, 0, (unsigned int)InitialCount) != 0)
	{
		return false;
	}
	Semaphore->MaximumCount = MaximumCount;
	return true;
}

bool ART::Threading::Platform::WaitSemaphore(
	void *Storage, const bool Block
)
{
	if(Storage == NULL)
	{
		return false;
	}

	auto Semaphore = (SemaphoreStorage *)Storage;
	int Result;
	do
	{
		Result = Block
			? sem_wait(&Semaphore->Semaphore)
			: sem_trywait(&Semaphore->Semaphore);
	} while(Result != 0 && errno == EINTR);
	return Result == 0;
}

bool ART::Threading::Platform::ReleaseSemaphore(
	void *Storage, const i32 Count
)
{
	if(Storage == NULL || Count <= 0)
	{
		return false;
	}

	auto Semaphore = (SemaphoreStorage *)Storage;
	for(i32 i = 0; i < Count; i++)
	{
		if(sem_post(&Semaphore->Semaphore) != 0)
		{
			return false;
		}
	}
	return true;
}

void ART::Threading::Platform::DestroySemaphore(void *Storage)
{
	if(Storage == NULL)
	{
		return;
	}
	auto Semaphore = (SemaphoreStorage *)Storage;
	sem_destroy(&Semaphore->Semaphore);
	Semaphore->MaximumCount = 0;
}

void ART::Threading::Platform::WaitAddress(
	volatile i32 *Address, const i32 Value
)
{
	syscall(
		SYS_futex, (i32 *)Address, FUTEX_WAIT_PRIVATE,
		Value, NULL, NULL, 0
	);
}

void ART::Threading::Platform::WakeAddressOne(volatile i32 *Address)
{
	syscall(
		SYS_futex, (i32 *)Address, FUTEX_WAKE_PRIVATE,
		1, NULL, NULL, 0
	);
}

void ART::Threading::Platform::WakeAddressAll(volatile i32 *Address)
{
	syscall(
		SYS_futex, (i32 *)Address, FUTEX_WAKE_PRIVATE,
		INT_MAX, NULL, NULL, 0
	);
}
