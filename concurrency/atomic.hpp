#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__i386__) || defined(__x86_64__)
#include <immintrin.h>
#endif

namespace ART::Threading::Detail
{
#ifndef _MSC_VER
	static_assert(__atomic_always_lock_free(sizeof(long), nullptr),
				  "ART synchronization requires lock-free long atomics");
#endif

	inline long LoadAcquire(const volatile long *Address)
	{
#ifdef _MSC_VER
		return _InterlockedCompareExchange(const_cast<volatile long *>(Address), 0, 0);
#else
		return __atomic_load_n(Address, __ATOMIC_ACQUIRE);
#endif
	}

	inline bool CompareExchangeAcquire(volatile long *Address, long Expected, long Desired)
	{
#ifdef _MSC_VER
		return _InterlockedCompareExchange(Address, Desired, Expected) == Expected;
#else
		return __atomic_compare_exchange_n(Address, &Expected, Desired, false, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
#endif
	}

	inline void StoreRelease(volatile long *Address, long Value)
	{
#ifdef _MSC_VER
		_InterlockedExchange(Address, Value);
#else
		__atomic_store_n(Address, Value, __ATOMIC_RELEASE);
#endif
	}

	inline void Pause()
	{
#if defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64))
		__yield();
#elif defined(_MSC_VER) || defined(__i386__) || defined(__x86_64__)
		_mm_pause();
#elif defined(__aarch64__) || defined(__arm__)
		__asm__ __volatile__("yield");
#else
		__asm__ __volatile__("" ::: "memory");
#endif
	}
} // namespace ART::Threading::Detail
