#include "spinlock.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

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
			_mm_pause();
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