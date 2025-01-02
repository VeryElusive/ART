#pragma once
#include "decl.hpp"

namespace ART
{
#ifdef CUSTOM_MEM_CALLBACK
	typedef void *(*HeapAllocCallback_t)(Size_t Size);
	typedef void *(*HeapReallocCallback_t)(void *Address, Size_t NewSize);
	typedef void (*HeapFreeCallback_t)(void *Address);

	inline HeapAllocCallback_t		HeapAllocCallback;
	inline HeapReallocCallback_t	HeapReallocCallback;
	inline HeapFreeCallback_t		HeapFreeCallback;
#endif

	void *Alloc(Size_t Size);

	void *Realloc(void *Address, Size_t Size);

	void Free(void *Address);
}
