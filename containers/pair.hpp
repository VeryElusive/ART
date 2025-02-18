#pragma once

#include "../common/decl.hpp"

namespace ART
{
	template <typename T1, typename T2>
	class Pair
	{
	public:
		T1 First;
		T2 Second;

		Pair() : First({}), Second({}) {}
		Pair(T1 One1, T2 Two2) : First(One1), Second(Two2) {}
	};

	template <typename T1, typename T2>
	inline ART::Pair<T1, T2> MakePair(T1 One, T2 Two)
	{
		ART::Pair<T1, T2> Ret(One, Two);
		return Ret;
	}
}