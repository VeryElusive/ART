#include "memory.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

namespace ART
{
	void *Memcpy(void *dest, const void *src, size_t size)
	{
		u8			*d = (u8 *)(dest);
		const u8	*s = (const u8 *)(src);

		while(size >= 32)
		{
			__m256i data = _mm256_loadu_si256((const __m256i *)(s));
			_mm256_storeu_si256((__m256i *)(d), data);

			d += 32;
			s += 32;
			size -= 32;
		}

		while(size >= 8)
		{
			*(u64 *)(d)  = *(u64 *)(s);

			s += 8;
			d += 8;
			size -= 8;
		}

		while(size >= 4)
		{
			*(u32 *)(d) = *(u32 *)(s);

			s += 4;
			d += 4;
			size -= 4;
		}

		while(size >= 2)
		{
			*(u16 *)(d) = *(u16 *)(s);

			s += 2;
			d += 2;
			size -= 2;
		}

		while(size >= 1)
		{
			*(u8 *)(d) = *(u8 *)(s);

			s += 1;
			d += 1;
			size -= 1;
		}

		return dest;
	}

	void *Memset(void *dest, u8 value, size_t size)
	{
		u8 *d = (u8 *)(dest);

		__m256i Fill32 = _mm256_set1_epi8((char)(value));

		while(size >= 32)
		{
			_mm256_storeu_si256((__m256i *)(d), fill);

			d += 32;
			size -= 32;
		}

		while(size--)
		{
			*d++ = value;
		}

		return dest;
	}
}