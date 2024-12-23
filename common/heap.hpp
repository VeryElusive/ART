#pragma once

#include "decl.hpp"

namespace ART
{
#ifdef CUSTOM_MEM_CALLBACK
	typedef void *(*HeapAllocCallback_t)(libutil_size Size);
	typedef void *(*HeapReallocCallback_t)(void *Address, libutil_size NewSize);
	typedef void (*HeapFreeCallback_t)(void *Address);

	HeapAllocCallback_t		HeapAllocCallback;
	HeapReallocCallback_t	HeapReallocCallback;
	HeapFreeCallback_t		HeapFreeCallback;
#endif

	void *Alloc(size_t Size);

	void *Realloc(void *Address, size_t Size);

	void Free(void *Address);
}