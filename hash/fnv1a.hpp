#pragma once

#ifndef ART_ALREADY_DEFINED

#include "../common/decl.hpp"
#include "../common/string.hpp"

namespace ART
{
	namespace FNV1A
	{
		typedef Size_t   FNV1A_t;

		constexpr FNV1A_t Basis = 0xCBF29CE484222325ULL;
		constexpr FNV1A_t Prime = 0x100000001B3ULL;

		consteval FNV1A_t HashConst(const char *String, const FNV1A_t Key = Basis)
		{
			return (String[0] == '\0') ? Key : HashConst(&String[1], (Key ^ FNV1A_t(String[0])) * Prime);
		}

		inline FNV1A_t Hash(const char *String, FNV1A_t uKey = Basis)
		{
			const Size_t Length = String::StringLength(String);

			for(Size_t i = 0U; i < Length; ++i)
			{
				uKey ^= String[i];
				uKey *= Prime;
			}

			return uKey;
		}
	}
}

#else
#include "ART/hash/fnv1a.hpp"
#endif