#pragma once

#include "decl.hpp"

namespace ART
{
#ifdef CUSTOM_MEM_CALLBACK
	typedef void *(*HeapAllocCallback_t)(Size_t Size);
	typedef void *(*HeapReallocCallback_t)(void *Address, Size_t NewSize);
	typedef void (*HeapFreeCallback_t)(void *Address);

	HeapAllocCallback_t		HeapAllocCallback;
	HeapReallocCallback_t	HeapReallocCallback;
	HeapFreeCallback_t		HeapFreeCallback;
#endif

	void *Alloc(Size_t Size);

	void *Realloc(void *Address, Size_t Size);

	void Free(void *Address);
}