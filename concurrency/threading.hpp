#pragma once

#include "../common/decl.hpp"
#include "spinlock.hpp"

namespace ART
{
	namespace Threading
	{
		using JobFunction_t = void (*)(void *Context);
		struct Batch;

		struct Job
		{
			JobFunction_t Function;
			void *Context;
			Batch *CompletionBatch;
		};

		struct Batch
		{
			volatile i32 Remaining;

			bool IsComplete() const;
			void Wait();
		};

		class SharedMutex
		{
		public:
			void Lock();
			void Unlock();
			void LockShared();
			void UnlockShared();

		private:
			volatile i32 State;
			volatile i32 WaitingWriters;
		};

		Size_t GetProcessorCount();

		namespace Detail
		{
			struct Worker
			{
				Ptr_t Thread;
				alignas(16) u8 StartContext[32];
			};

			struct ThreadPoolState
			{
				Worker *Workers;
				Job *Queue;
				Size_t WorkerCapacity;
				Size_t QueueCapacity;
				Size_t WorkerCount;
				Size_t WorkerLimit;
				Size_t QueueHead;
				Size_t QueueTail;
				Size_t QueueCount;
				Spinlock QueueLock;
				alignas(16) u8 QueueSemaphore[64];
				alignas(16) u8 ExecutionSemaphore[64];
				volatile i32 Shutdown;
				bool Initialized;
			};

			bool SetThreadPoolWorkerLimit(
				ThreadPoolState *State,
				Worker *Workers, Size_t WorkerCapacity,
				Job *Queue, Size_t QueueCapacity,
				Size_t WorkerLimit
			);
			bool DispatchThreadPoolJobs(
				ThreadPoolState *State,
				Worker *Workers, Size_t WorkerCapacity,
				Job *Queue, Size_t QueueCapacity,
				const Job *Jobs, Size_t JobCount, Batch *Batch
			);
			void ShutdownThreadPool(
				ThreadPoolState *State,
				Worker *Workers, Size_t WorkerCapacity,
				Job *Queue, Size_t QueueCapacity
			);
		}

		template<Size_t WorkerCapacity, Size_t QueueCapacity>
		class ThreadPool
		{
		public:
			bool SetWorkerLimit(const Size_t WorkerLimit)
			{
				return Detail::SetThreadPoolWorkerLimit(
					&State,
					Workers, WorkerCapacity,
					Queue, QueueCapacity,
					WorkerLimit
				);
			}

			bool Dispatch(
				const Job *Jobs, const Size_t JobCount, Batch *Batch = NULL
			)
			{
				return Detail::DispatchThreadPoolJobs(
					&State,
					Workers, WorkerCapacity,
					Queue, QueueCapacity,
					Jobs, JobCount, Batch
				);
			}

			Size_t GetWorkerCount() const
			{
				return State.WorkerCount;
			}

			Size_t GetWorkerLimit() const
			{
				return State.WorkerLimit;
			}

			void Shutdown()
			{
				Detail::ShutdownThreadPool(
					&State,
					Workers, WorkerCapacity,
					Queue, QueueCapacity
				);
			}

		private:
			Detail::ThreadPoolState State;
			Detail::Worker Workers[WorkerCapacity];
			Job Queue[QueueCapacity];
		};
	}
}
