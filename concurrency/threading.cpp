#include "threading.hpp"
#include "threading_platform.hpp"

#include "../common/memory.hpp"

namespace
{
	void CompleteBatch(ART::Threading::Batch *Batch)
	{
		if(Batch != NULL
			&& ART::Threading::Platform::AtomicDecrement(&Batch->Remaining) == 0)
		{
			ART::Threading::Platform::WakeAddressAll(&Batch->Remaining);
		}
	}

	void RunThreadPoolWorker(void *Parameter)
	{
		auto State = (ART::Threading::Detail::ThreadPoolState *)Parameter;
		for(;;)
		{
			if(ART::Threading::Platform::WaitSemaphore(
				State->QueueSemaphore, true
			) == false
				|| ART::Threading::Platform::AtomicCompareExchange(
					&State->Shutdown, 0, 0
				) != 0)
			{
				break;
			}

			ART::Threading::Job Job;
			ART::Memset(&Job, 0, sizeof(Job));
			State->QueueLock.Lock();
			if(State->QueueCount != 0)
			{
				Job = State->Queue[State->QueueHead];
				State->QueueHead = (State->QueueHead + 1) % State->QueueCapacity;
				State->QueueCount--;
			}
			State->QueueLock.Unlock();

			if(Job.Function == NULL)
			{
				continue;
			}

			if(ART::Threading::Platform::WaitSemaphore(
				State->ExecutionSemaphore, true
			) == false)
			{
				break;
			}

			Job.Function(Job.Context);
			ART::Threading::Platform::ReleaseSemaphore(
				State->ExecutionSemaphore, 1
			);
			CompleteBatch(Job.CompletionBatch);
		}
	}

	bool InitializeThreadPool(
		ART::Threading::Detail::ThreadPoolState *State,
		ART::Threading::Detail::Worker *Workers, const Size_t WorkerCapacity,
		ART::Threading::Job *Queue, const Size_t QueueCapacity
	)
	{
		if(State->Initialized)
		{
			return true;
		}

		if(WorkerCapacity == 0 || QueueCapacity == 0
			|| WorkerCapacity > 0x7FFFFFFF
			|| QueueCapacity > 0x7FFFFFFF
			|| WorkerCapacity + QueueCapacity > 0x7FFFFFFF
			|| ART::Threading::Platform::Initialize() == false)
		{
			return false;
		}

		State->Workers = Workers;
		State->Queue = Queue;
		State->WorkerCapacity = WorkerCapacity;
		State->QueueCapacity = QueueCapacity;

		const auto QueueInitialized =
			ART::Threading::Platform::InitializeSemaphore(
				State->QueueSemaphore, 0,
				(i32)(QueueCapacity + WorkerCapacity)
			);
		const auto ExecutionInitialized = QueueInitialized
			&& ART::Threading::Platform::InitializeSemaphore(
				State->ExecutionSemaphore, 0, (i32)WorkerCapacity
			);
		if(ExecutionInitialized == false)
		{
			if(QueueInitialized)
			{
				ART::Threading::Platform::DestroySemaphore(
					State->QueueSemaphore
				);
			}
			ART::Memset(State, 0, sizeof(*State));
			return false;
		}

		State->Initialized = true;
		return true;
	}
}

bool ART::Threading::Batch::IsComplete() const
{
	return Platform::AtomicCompareExchange(
		const_cast<volatile i32 *>(&Remaining), 0, 0
	) == 0;
}

void ART::Threading::Batch::Wait()
{
	for(;;)
	{
		const auto Value = Platform::AtomicCompareExchange(&Remaining, 0, 0);
		if(Value == 0)
		{
			break;
		}
		Platform::WaitAddress(&Remaining, Value);
	}
}

void ART::Threading::SharedMutex::Lock()
{
	Platform::Initialize();
	Platform::AtomicIncrement(&WaitingWriters);
	for(;;)
	{
		const auto CurrentState = Platform::AtomicCompareExchange(&State, -1, 0);
		if(CurrentState == 0)
		{
			break;
		}
		Platform::WaitAddress(&State, CurrentState);
	}
	Platform::AtomicDecrement(&WaitingWriters);
}

void ART::Threading::SharedMutex::Unlock()
{
	Platform::AtomicExchange(&State, 0);
	Platform::WakeAddressAll(&State);
}

void ART::Threading::SharedMutex::LockShared()
{
	Platform::Initialize();
	for(;;)
	{
		while(Platform::AtomicCompareExchange(&WaitingWriters, 0, 0) != 0)
		{
			const auto CurrentState = Platform::AtomicCompareExchange(&State, 0, 0);
			Platform::WaitAddress(&State, CurrentState);
		}

		const auto Readers = Platform::AtomicCompareExchange(&State, 0, 0);
		if(Readers < 0)
		{
			Platform::WaitAddress(&State, Readers);
			continue;
		}

		if(Platform::AtomicCompareExchange(&WaitingWriters, 0, 0) == 0
			&& Platform::AtomicCompareExchange(
				&State, Readers + 1, Readers
			) == Readers)
		{
			break;
		}
	}
}

void ART::Threading::SharedMutex::UnlockShared()
{
	if(Platform::AtomicDecrement(&State) == 0)
	{
		Platform::WakeAddressAll(&State);
	}
}

Size_t ART::Threading::GetProcessorCount()
{
	const auto Count = Platform::GetProcessorCount();
	return Count == 0 ? 1 : Count;
}

bool ART::Threading::Detail::SetThreadPoolWorkerLimit(
	ThreadPoolState *State,
	Worker *Workers, const Size_t WorkerCapacity,
	Job *Queue, const Size_t QueueCapacity,
	Size_t WorkerLimit
)
{
	if(WorkerLimit > WorkerCapacity)
	{
		WorkerLimit = WorkerCapacity;
	}

	if(WorkerLimit == 0 && State->Initialized == false)
	{
		return true;
	}

	if(InitializeThreadPool(
		State, Workers, WorkerCapacity, Queue, QueueCapacity
	) == false)
	{
		return false;
	}

	while(State->WorkerCount < WorkerLimit)
	{
		auto &Worker = Workers[State->WorkerCount];
		if(Platform::CreateThread(
			&Worker.Thread, Worker.StartContext,
			RunThreadPoolWorker, State
		) == false)
		{
			break;
		}
		State->WorkerCount++;
	}

	if(WorkerLimit > State->WorkerCount)
	{
		WorkerLimit = State->WorkerCount;
	}

	while(State->WorkerLimit > WorkerLimit)
	{
		if(Platform::WaitSemaphore(State->ExecutionSemaphore, false) == false)
		{
			break;
		}
		State->WorkerLimit--;
	}

	while(State->WorkerLimit < WorkerLimit)
	{
		if(Platform::ReleaseSemaphore(State->ExecutionSemaphore, 1) == false)
		{
			break;
		}
		State->WorkerLimit++;
	}

	return State->WorkerLimit == WorkerLimit;
}

bool ART::Threading::Detail::DispatchThreadPoolJobs(
	ThreadPoolState *State,
	Worker *Workers, const Size_t WorkerCapacity,
	Job *Queue, const Size_t QueueCapacity,
	const Job *Jobs, const Size_t JobCount, Batch *Batch
)
{
	if(Jobs == NULL || JobCount == 0
		|| InitializeThreadPool(
			State, Workers, WorkerCapacity, Queue, QueueCapacity
		) == false
		|| State->WorkerLimit == 0)
	{
		return false;
	}

	if(Batch != NULL
		&& Platform::AtomicCompareExchange(&Batch->Remaining, 0, 0) != 0)
	{
		return false;
	}
	for(Size_t i = 0; i < JobCount; i++)
	{
		if(Jobs[i].Function == NULL)
		{
			return false;
		}
	}

	State->QueueLock.Lock();
	if(State->QueueCount + JobCount > State->QueueCapacity)
	{
		State->QueueLock.Unlock();
		return false;
	}

	if(Batch != NULL)
	{
		Platform::AtomicExchange(&Batch->Remaining, (i32)JobCount);
	}

	const auto OldQueueTail = State->QueueTail;
	for(Size_t i = 0; i < JobCount; i++)
	{
		State->Queue[State->QueueTail] = Jobs[i];
		State->Queue[State->QueueTail].CompletionBatch = Batch;
		State->QueueTail = (State->QueueTail + 1) % State->QueueCapacity;
		State->QueueCount++;
	}

	if(Platform::ReleaseSemaphore(State->QueueSemaphore, (i32)JobCount))
	{
		State->QueueLock.Unlock();
		return true;
	}

	State->QueueTail = OldQueueTail;
	State->QueueCount -= JobCount;
	State->QueueLock.Unlock();
	if(Batch != NULL)
	{
		Platform::AtomicExchange(&Batch->Remaining, 0);
	}
	return false;
}

void ART::Threading::Detail::ShutdownThreadPool(
	ThreadPoolState *State,
	Worker *Workers, const Size_t WorkerCapacity,
	Job *Queue, const Size_t QueueCapacity
)
{
	if(State->Initialized == false)
	{
		return;
	}

	Platform::AtomicExchange(&State->Shutdown, 1);
	while(State->WorkerLimit < State->WorkerCount)
	{
		if(Platform::ReleaseSemaphore(State->ExecutionSemaphore, 1) == false)
		{
			break;
		}
		State->WorkerLimit++;
	}

	if(State->WorkerCount != 0)
	{
		Platform::ReleaseSemaphore(
			State->QueueSemaphore, (i32)State->WorkerCount
		);
	}

	for(Size_t i = 0; i < State->WorkerCount; i++)
	{
		Platform::JoinThread(Workers[i].Thread);
	}

	Platform::DestroySemaphore(State->QueueSemaphore);
	Platform::DestroySemaphore(State->ExecutionSemaphore);
	ART::Memset(Workers, 0, WorkerCapacity * sizeof(Worker));
	ART::Memset(Queue, 0, QueueCapacity * sizeof(Job));
	ART::Memset(State, 0, sizeof(*State));
}
