#include "memory.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

namespace ART
{
	/// <summary>
	/// copies memory from one address to another
	/// </summary>
	/// <param name="Dest">destination address</param>
	/// <param name="Src">source address</param>
	/// <param name="Size">size in bytes</param>
	void Memcpy(void *Dest, const void *Src, size_t Size)
	{
		u8			*d = (u8 *)(Dest);
		const u8	*s = (const u8 *)(Src);

		while(Size >= 32)
		{
			__m256i data = _mm256_loadu_si256((const __m256i *)(s));
			_mm256_storeu_si256((__m256i *)(d), data);

			d += 32;
			s += 32;
			Size -= 32;
		}

		while(Size >= 8)
		{
			*(u64 *)(d)  = *(u64 *)(s);

			s += 8;
			d += 8;
			Size -= 8;
		}

		while(Size >= 4)
		{
			*(u32 *)(d) = *(u32 *)(s);

			s += 4;
			d += 4;
			Size -= 4;
		}

		while(Size >= 2)
		{
			*(u16 *)(d) = *(u16 *)(s);

			s += 2;
			d += 2;
			Size -= 2;
		}

		while(Size >= 1)
		{
			*(u8 *)(d) = *(u8 *)(s);

			s += 1;
			d += 1;
			Size -= 1;
		}
	}

	/// <summary>
	/// copies memory from one address to another, but handles overlapping memory.
	/// </summary>
	/// <param name="Dest">destination address</param>
	/// <param name="Src">source address</param>
	/// <param name="Size">size in bytes</param>
	void Memmove(void *Dest, const void *Src, size_t Size)
	{
		u8 *d = (u8 *)(Dest);
		const u8 *s = (const u8 *)(Src);

		if(d < s || d >= s + Size) 
		{
			// No overlap, can safely copy as in Memcpy
			Memcpy(Dest, Src, Size);
		}
		else 
		{
			// Overlapping memory, copy from end to avoid overwriting
			d += Size;
			s += Size;

			while(Size >= 32)
			{
				s -= 32;
				d -= 32;
				__m256i data = _mm256_loadu_si256((const __m256i *)(s));
				_mm256_storeu_si256((__m256i *)(d), data);

				Size -= 32;
			}

			while(Size >= 8)
			{
				s -= 8;
				d -= 8;
				*(u64 *)(d) = *(u64 *)(s);

				Size -= 8;
			}

			while(Size >= 4)
			{
				s -= 4;
				d -= 4;
				*(u32 *)(d) = *(u32 *)(s);

				Size -= 4;
			}

			while(Size >= 2)
			{
				s -= 2;
				d -= 2;
				*(u16 *)(d) = *(u16 *)(s);

				Size -= 2;
			}

			while(Size >= 1)
			{
				s -= 1;
				d -= 1;
				*(u8 *)(d) = *(u8 *)(s);

				Size -= 1;
			}
		}
	}


	/// <summary>
	/// sets all bytes in a selected memory region to a particular value.
	/// </summary>
	/// <param name="Dest">Destination address</param>
	/// <param name="Value">Value in which bytes are set to</param>
	/// <param name="Size">Size in bytes</param>
	void Memset(void *Dest, u8 Value, size_t Size)
	{
		u8 *d = (u8 *)(Dest);

		__m256i Fill32 = _mm256_set1_epi8((char)(Value));

		while(size >= 32)
		{
			_mm256_storeu_si256((__m256i *)(d), Fill32);

			d += 32;
			size -= 32;
		}

		while(Size--)
		{
			*d++ = Value;
		}
	}
}