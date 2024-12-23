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
	/// <summary>
	/// allocates memory to the heap.
	/// </summary>
	/// <param name="Size">Size of memory allocated</param>
	/// <returns>Address of the heap allocated memory</returns>
	void *Alloc(size_t Size)
	{
#ifdef CUSTOM_MEM_CALLBACK
		if(HeapAllocCallback)
		{
			return HeapAllocCallback(Size);
		}
#endif
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

	/// <summary>
	/// Resizes memory allocated to the heap.
	/// </summary>
	/// <param name="Address">Address of the old heap memory location</param>
	/// <param name="Size">New size to resize</param>
	/// <returns>Address of new heap allocation</returns>
	void *Realloc(void *Address, size_t Size)
	{
#ifdef CUSTOM_MEM_CALLBACK
		if(HeapReallocCallback)
		{
			return HeapReallocCallback(Address, Size);
		}
#endif

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

	/// <summary>
	/// Marks memory on the heap as free.
	/// </summary>
	/// <param name="Address">Address of heap allocation</param>
	void Free(void *Address)
	{
#ifdef CUSTOM_MEM_CALLBACK
		if(HeapFreeCallback)
		{
			return HeapFreeCallback(Address);
		}
#endif

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
