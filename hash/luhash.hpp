#pragma once
#ifndef ART_ALREADY_DEFINED

#include "ART/common/decl.hpp"

namespace ART
{
	namespace LUHash
	{
		u32 Hash2(const void *Data, u32 Seed, Size_t Length);
		consteval u32 Hash2Const(const void *Data, u32 Seed, Size_t Length);
	}
}
#else
#include "ART/hash/luhash.hpp"
#endif