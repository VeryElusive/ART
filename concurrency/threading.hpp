#pragma once

#include "../common/decl.hpp"
#include "../common/memory.hpp"
#include "mutex.hpp"
#include "spinlock.hpp"

#include <new>

namespace ART
{
	namespace Threading
	{
		using ThreadID_t = Ptr_t;
		using JobFunction_t = void (*)(void *Context);
		inline constexpr Size_t ThreadCallableStorageSize = 128;
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

		Size_t GetProcessorCount();

		namespace ThisThread
		{
			ThreadID_t GetID();
			void Yield();
		}

		namespace Detail
		{
			template<typename Type>
			struct RemoveReference
			{
				using Result = Type;
			};

			template<typename Type>
			struct RemoveReference<Type &>
			{
				using Result = Type;
			};

			template<typename Type>
			struct RemoveReference<Type &&>
			{
				using Result = Type;
			};

			template<typename Type>
			struct RemoveConst
			{
				using Result = Type;
			};

			template<typename Type>
			struct RemoveConst<const Type>
			{
				using Result = Type;
			};

			template<typename Type>
			struct RemoveVolatile
			{
				using Result = Type;
			};

			template<typename Type>
			struct RemoveVolatile<volatile Type>
			{
				using Result = Type;
			};

			template<typename Type>
			struct RemoveCV
			{
				using Result = typename RemoveVolatile<
				typename RemoveConst<Type>::Result
				>::Result;
			};

			template<typename Type>
			struct DecayValue
			{
				using Result = typename RemoveCV<Type>::Result;
			};

			template<typename Type, Size_t Count>
			struct DecayValue<Type[Count]>
			{
				using Result = Type *;
			};

			template<typename Type>
			struct DecayValue<Type[]>
			{
				using Result = Type *;
			};

			template<typename ReturnType, typename... Arguments>
			struct DecayValue<ReturnType(Arguments...)>
			{
				using Result = ReturnType (*)(Arguments...);
			};

			template<typename ReturnType, typename... Arguments>
			struct DecayValue<ReturnType(Arguments...) noexcept>
			{
				using Result = ReturnType (*)(Arguments...) noexcept;
			};

			template<typename Type>
			using DecayType = typename DecayValue<
				typename RemoveReference<Type>::Result
			>::Result;

			template<Size_t... Values>
			struct IndexSequence
			{
			};

			template<Size_t Count, Size_t... Values>
			struct MakeIndexSequence
				: MakeIndexSequence<Count - 1, Count - 1, Values...>
			{
			};

			template<Size_t... Values>
			struct MakeIndexSequence<0, Values...>
			{
				using Result = IndexSequence<Values...>;
			};

			template<typename Type>
			Type &&MoveStored(Type &Value)
			{
				return (Type &&)Value;
			}

			template<Size_t Index, typename Type>
			struct ThreadArgument
			{
				template<typename InputType>
				explicit ThreadArgument(InputType &&Input)
					: Value((InputType &&)Input)
				{
				}

				Type Value;
			};

			template<typename Indices, typename... ArgumentTypes>
			struct ThreadArguments;

			template<Size_t... Indices, typename... ArgumentTypes>
			struct ThreadArguments<
				IndexSequence<Indices...>, ArgumentTypes...
			> : ThreadArgument<Indices, ArgumentTypes>...
			{
				template<typename... InputTypes>
				explicit ThreadArguments(InputTypes &&...Inputs)
					: ThreadArgument<Indices, ArgumentTypes>(
						(InputTypes &&)Inputs
					)...
				{
				}
			};

			template<typename CallableType, typename... ArgumentTypes>
			struct ThreadInvocation
			{
				using Indices = typename MakeIndexSequence<
					sizeof...(ArgumentTypes)
				>::Result;
				using Arguments = ThreadArguments<Indices, ArgumentTypes...>;

				template<typename CallableInput, typename... ArgumentInputs>
				ThreadInvocation(
					CallableInput &&CallableValue,
					ArgumentInputs &&...ArgumentValues
				)
					: CallableObject((CallableInput &&)CallableValue),
					ArgumentsObject((ArgumentInputs &&)ArgumentValues...)
				{
				}

				static void InvokeStored(void *Storage)
				{
					((ThreadInvocation *)Storage)->Invoke(Indices());
				}

				static void DestroyStored(void *Storage)
				{
					((ThreadInvocation *)Storage)->~ThreadInvocation();
				}

			private:
				template<Size_t... IndicesValue>
				void Invoke(IndexSequence<IndicesValue...>)
				{
					(MoveStored(CallableObject))(
						MoveStored(
							static_cast<ThreadArgument<
								IndicesValue, ArgumentTypes
							> &>(ArgumentsObject).Value
						)...
					);
				}

				CallableType CallableObject;
				Arguments ArgumentsObject;
			};

			struct ThreadState
			{
				void (*Invoke)(void *Storage);
				void (*Destroy)(void *Storage);
				alignas(16) u8 PlatformStartContext[32];
				alignas(16) u8 CallableStorage[ThreadCallableStorageSize];
			};

			ThreadState *AllocateThreadState();
			void FreeThreadState(ThreadState *State);
			bool StartThreadState(
				ThreadState *State, Ptr_t *Thread, ThreadID_t *ThreadID
			);

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

		class Thread
		{
		public:
			Thread();

			template<typename Callable, typename... Arguments>
			explicit Thread(Callable &&CallableValue, Arguments &&...ArgumentValues)
				: Handle(0), ID(0)
			{
				Start(
					(Callable &&)CallableValue,
					(Arguments &&)ArgumentValues...
				);
			}

			~Thread();

			Thread(Thread &) = delete;
			Thread(const Thread &) = delete;
			Thread &operator=(const Thread &) = delete;
			Thread(Thread &&Other);
			Thread &operator=(Thread &&Other);

			bool Joinable() const;
			void Join();
			void Detach();
			ThreadID_t GetID() const;

		private:
			template<typename Callable, typename... Arguments>
			bool Start(Callable &&CallableValue, Arguments &&...ArgumentValues)
			{
				using Invocation = Detail::ThreadInvocation<
					Detail::DecayType<Callable>,
					Detail::DecayType<Arguments>...
				>;
				static_assert(
					sizeof(Invocation) <= ThreadCallableStorageSize,
					"thread callable and arguments exceed inline storage"
				);
				static_assert(
					alignof(Invocation) <= 16,
					"thread callable alignment exceeds inline storage"
				);

				auto State = Detail::AllocateThreadState();
				if(State == NULL)
				{
					return false;
				}

				::new ((void *)State->CallableStorage) Invocation(
					(Callable &&)CallableValue,
					(Arguments &&)ArgumentValues...
				);
				State->Invoke = Invocation::InvokeStored;
				State->Destroy = Invocation::DestroyStored;

				if(Detail::StartThreadState(State, &Handle, &ID) == false)
				{
					State->Destroy(State->CallableStorage);
					Detail::FreeThreadState(State);
					return false;
				}
				return true;
			}

			Ptr_t Handle;
			ThreadID_t ID;
		};

		template<Size_t WorkerCapacity, Size_t QueueCapacity>
		class ThreadPool
		{
		public:
			ThreadPool()
			{
				ART::Memset(&State, 0, sizeof(State));
				ART::Memset(Workers, 0, sizeof(Workers));
				ART::Memset(Queue, 0, sizeof(Queue));
			}
			ThreadPool(const ThreadPool &) = delete;
			ThreadPool &operator=(const ThreadPool &) = delete;
			ThreadPool(ThreadPool &&) = delete;
			ThreadPool &operator=(ThreadPool &&) = delete;

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
