#pragma once

namespace ART
{
	class Spinlock
	{
	public:
		enum class Initialization
		{
			Unlocked
		};

		Spinlock() = default;
		explicit constexpr Spinlock(Initialization) noexcept : LockSwitch(0) {}

		void Lock();
		bool TryLock();
		void Unlock();

	private:
		volatile long LockSwitch;
	};
}
