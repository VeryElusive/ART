#include "heap.hpp"
#include "memory.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace ART
{
	void *Alloc(u64 Size)
	{
		if(Size == NULL)
		{
			return NULL;
		}
#ifdef _WIN32
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Size);
#else
		return mmap(NULL, Size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
	}

	void *Realloc(void *Address, u64 Size)
	{
		if(Size == NULL)
		{
			return NULL;
		}

#ifdef _WIN32
		return HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Address, Size);
#else
		void *NewAddress = mmap(NULL, Size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if(Address && NewAddress)
		{
			Memcpy(NewAddress, Address, Size);
			munmap(Address, Size);
		}
		return NewAddress;
#endif
	}

	void Free(void *Address)
	{
		if(Address == NULL)
		{
			return;
		}

#ifdef _WIN32
		HeapFree(GetProcessHeap(), 0, Address);
#else
		munmap(Address, 0);
#endif
	}
}
