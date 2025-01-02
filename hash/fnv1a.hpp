#pragma once

#include "../common/decl.hpp"
#include "../common/string.hpp"

namespace ART
{
	namespace FNV1A
	{
#define BASIS 0xCBF29CE484222325ULL
#define PRIME 0x100000001B3ULL

		consteval u64 HashConst(const char *String, const u64 Key = BASIS)
		{
			return (String[0] == '\0') ? Key : HashConst(&String[1], (Key ^ (u64)(String[0])) * PRIME);
		}

		inline u64 Hash(const char *String, u64 Key = BASIS)
		{
			const Size_t Length = ART::StringLength(String);

			for(Size_t i = 0U; i < Length; ++i)
			{
				Key ^= String[i];
				Key *= PRIME;
			}

			return Key;
		}

		inline u64 Hash(u8 *Data, Size_t Length, u64 Key = BASIS)
		{
			for(Size_t i = 0U; i < Length; ++i)
			{
				Key ^= Data[i];
				Key *= PRIME;
			}

			return Key;
		}

#undef BASIS
#undef PRIME
	}
}
