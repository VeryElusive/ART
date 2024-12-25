#pragma once

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
			volatile long Lock;
		};
	}
}