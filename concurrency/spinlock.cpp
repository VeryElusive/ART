#include "spinlock.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif (defined(__GNUC__) || defined(__clang__)) \
	&& (defined(__i386__) || defined(__x86_64__))
#include <immintrin.h>
#endif

namespace
{
	void PauseProcessor()
	{
#if defined(_MSC_VER) || defined(__i386__) || defined(__x86_64__)
		_mm_pause();
#elif defined(__aarch64__)
		__asm__ __volatile__("yield");
#else
		__asm__ __volatile__("" ::: "memory");
#endif
	}
}

void ART::Spinlock::Lock()
{
#ifdef _MSC_VER
	while(_InterlockedCompareExchange(&LockSwitch, 1, 0) != 0)
#else
	while(__sync_val_compare_and_swap(&LockSwitch, 1, 0) != 0)	
#endif
	{
		while(LockSwitch != 0)
		{
			PauseProcessor();
		}
	}
}

void ART::Spinlock::Unlock()
{
#ifdef _MSC_VER
	_InterlockedExchange(&LockSwitch, 0);
#else
	__atomic_store_n(&LockSwitch, 0, __ATOMIC_RELEASE);
#endif
}
