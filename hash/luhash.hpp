#pragma once

#include "ART/common/decl.hpp"

namespace ART
{
	namespace Hash
	{
		namespace LUHash
		{
			u32 Hash2(const void *Data, u32 Seed, Size_t Length);
		}
	}
}