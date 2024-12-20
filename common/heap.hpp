#pragma once

#include "decl.hpp"

namespace ART
{
	void *Alloc(size_t Size);

	void *Realloc(void *Address, size_t Size);

	void Free(void *Address);
}