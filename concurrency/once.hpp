#pragma once

#include "atomic.hpp"

namespace ART::Threading
{
	class Once
	{
	public:
		constexpr Once() noexcept = default;
		Once(const Once &) = delete;
		Once &operator=(const Once &) = delete;

		template <typename Initializer>
		bool Execute(Initializer &&Initialize)
		{
			for (;;)
			{
				const long Current = Detail::LoadAcquire(&State);
				if (Current == Initialized)
					return true;
				if (Current == Uninitialized && Detail::CompareExchangeAcquire(&State, Uninitialized, Initializing))
				{
					Completion Publish{State};
					Publish.Succeeded = static_cast<bool>(Initialize());
					return Publish.Succeeded;
				}
				Detail::Pause();
			}
		}

	private:
		static constexpr long Uninitialized = 0, Initializing = 1, Initialized = 2;
		volatile long State = Uninitialized;

		struct Completion
		{
			volatile long &State;
			bool Succeeded = false;

			~Completion()
			{
				Detail::StoreRelease(&State, Succeeded ? Initialized : Uninitialized);
			}
		};
	};
}
