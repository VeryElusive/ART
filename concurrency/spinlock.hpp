#pragma once

#ifndef ART_ALREADY_DEFINED
namespace ART
{
	namespace Concurrency
	{
		class Spinlock
		{
		public:
			void Lock();
			void Unlock();
		private:
			volatile long LockSwitch;
		};
	}
}
#else
#include "ART/concurrency/spinlock.hpp"
#endif