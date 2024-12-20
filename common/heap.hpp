#pragma once

#include "decl.hpp"

namespace ART
{
	void *Alloc(u64 Size);

	void *Realloc(void *Address, u64 Size);

	void Free(void *Address);
}