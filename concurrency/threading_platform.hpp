#pragma once

#include "../common/decl.hpp"

namespace ART
{
	namespace Threading
	{
		namespace Platform
		{
			using ThreadEntry_t = void (*)(void *Context);

			bool Initialize();
			Size_t GetProcessorCount();

			i32 AtomicCompareExchange(
				volatile i32 *Address, i32 Exchange, i32 Comparand
			);
			i32 AtomicExchange(volatile i32 *Address, i32 Value);
			i32 AtomicIncrement(volatile i32 *Address);
			i32 AtomicDecrement(volatile i32 *Address);
			void Pause();

			bool CreateThread(
				Ptr_t *Thread, void *StartContext,
				ThreadEntry_t Entry, void *Context
			);
			void JoinThread(Ptr_t Thread);

			bool InitializeSemaphore(
				void *Storage, i32 InitialCount, i32 MaximumCount
			);
			bool WaitSemaphore(void *Storage, bool Block);
			bool ReleaseSemaphore(void *Storage, i32 Count);
			void DestroySemaphore(void *Storage);

			void WaitAddress(volatile i32 *Address, i32 Value);
			void WakeAddressAll(volatile i32 *Address);
		}
	}
}
