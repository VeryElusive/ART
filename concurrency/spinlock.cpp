#include "spinlock.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

void ART::Spinlock::Lock()
{
	while(_InterlockedCompareExchange(&LockSwitch, 1, 0) != 0)
	{
		while(LockSwitch != 0)
		{
			_mm_pause();
		}
	}
}

void ART::Spinlock::Unlock()
{
	_InterlockedExchange(&LockSwitch, 0);
}