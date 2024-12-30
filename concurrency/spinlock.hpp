#pragma once

namespace ART
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
