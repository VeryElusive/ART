#include "memory.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

namespace ART
{
	typedef enum CPU_FLAGS
	{
		CPU_NONE = 0,
		CPU_SSE2 = 1 << 0,
		CPU_AVX2 = 1 << 1

	} CPU_FLAGS;

	inline CPU_FLAGS CPUFlags = CPU_NONE;
	inline bool CPUFLagsHasBeenChecked = FALSE;

	static void Cpuid(int out[4], int leaf)
	{
		__cpuid(out, leaf);
	}

	static void CpuidEx(int out[4], int leaf, int subleaf)
	{
		__cpuidex(out, leaf, subleaf);
	}

	CPU_FLAGS GetCpuFlags()
	{
		int CpuInfo[4];
		CPU_FLAGS Flags = CPU_NONE;

		Cpuid(CpuInfo, 1);

		if (CpuInfo[3] & (1 << 26))
			Flags = (CPU_FLAGS)(Flags | CPU_SSE2);

		int HasAVX     = (CpuInfo[2] & (1 << 28)) != 0;
		int HasOSXSAVE = (CpuInfo[2] & (1 << 27)) != 0;

		if (HasAVX && HasOSXSAVE)
		{
			u64 Xcr0 = _xgetbv(0);

			if ((Xcr0 & 0x6) == 0x6)
			{
				CpuidEx(CpuInfo, 7, 0);

				if (CpuInfo[1] & (1 << 5))
				{
					Flags = (CPU_FLAGS)(Flags | CPU_AVX2);
				}
			}
		}

		return Flags;
	}

	/// <summary>
	/// copies memory from one address to another
	/// </summary>
	/// <param name="Dest">destination address</param>
	/// <param name="Src">source address</param>
	/// <param name="Size">size in bytes</param>
	void Memcpy(void *Dest, const void *Src, size_t Size)
	{
		if(CPUFLagsHasBeenChecked == FALSE)
		{
			CPUFLagsHasBeenChecked = TRUE;
			CPUFlags = GetCpuFlags();
		}

		u8 *d = (u8 *)Dest;
		const u8 *s = (const u8 *)Src;

		if(Size == 0 || d == s) return;

		if(CPUFlags & CPU_AVX2)
		{
			while(Size >= 32)
			{
				__m256i v = _mm256_loadu_si256((const __m256i *)s);
				_mm256_storeu_si256((__m256i *)d, v);
				d += 32;
				s += 32;
				Size -= 32;
			}
		}

		if(CPUFlags & CPU_SSE2)
		{
			while(Size >= 16)
			{
				__m128i v = _mm_loadu_si128((const __m128i *)s);
				_mm_storeu_si128((__m128i *)d, v);
				d += 16;
				s += 16;
				Size -= 16;
			}
		}

		if(Size >= 8)
		{
			*(u64 *)d = *(u64 *)s;
			d += 8; s += 8; Size -= 8;
		}

		if(Size >= 4)
		{
			*(u32 *)d = *(u32 *)s;
			d += 4; s += 4; Size -= 4;
		}

		if(Size >= 2)
		{
			*(u16 *)d = *(u16 *)s;
			d += 2; s += 2; Size -= 2;
		}

		if(Size >= 1)
		{
			*d = *s;
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
		if(CPUFLagsHasBeenChecked == FALSE)
		{
			CPUFLagsHasBeenChecked = TRUE;
			CPUFlags = GetCpuFlags();
		}

		u8 *d = (u8 *)Dest;
		const u8 *s = (const u8 *)Src;

		if(d == s || Size == 0) return;

		if(d < s)
		{
			if(CPUFlags & CPU_AVX2)
			{
				while(Size >= 32)
				{
					_mm256_storeu_si256((__m256i *)d, _mm256_loadu_si256((const __m256i *)s));
					d += 32; s += 32; Size -= 32;
				}
			}
			if(CPUFlags & CPU_SSE2)
			{
				while(Size >= 16)
				{
					_mm_storeu_si128((__m128i *)d, _mm_loadu_si128((const __m128i *)s));
					d += 16; s += 16; Size -= 16;
				}
			}
			if(Size >= 8) { *(u64 *)d = *(u64 *)s; d += 8; s += 8; Size -= 8; }
			if(Size >= 4) { *(u32 *)d = *(u32 *)s; d += 4; s += 4; Size -= 4; }
			if(Size >= 2) { *(u16 *)d = *(u16 *)s; d += 2; s += 2; Size -= 2; }
			if(Size >= 1) { *d = *s; }
		}
		else
		{
			d += Size;
			s += Size;

			if(CPUFlags & CPU_AVX2)
			{
				while(Size >= 32)
				{
					d -= 32; s -= 32; Size -= 32;
					_mm256_storeu_si256((__m256i *)d, _mm256_loadu_si256((const __m256i *)s));
				}
			}
			if(CPUFlags & CPU_SSE2)
			{
				while(Size >= 16)
				{
					d -= 16; s -= 16; Size -= 16;
					_mm_storeu_si128((__m128i *)d, _mm_loadu_si128((const __m128i *)s));
				}
			}
			if(Size >= 8) { d -= 8; s -= 8; Size -= 8; *(u64 *)d = *(u64 *)s; }
			if(Size >= 4) { d -= 4; s -= 4; Size -= 4; *(u32 *)d = *(u32 *)s; }
			if(Size >= 2) { d -= 2; s -= 2; Size -= 2; *(u16 *)d = *(u16 *)s; }
			if(Size >= 1) { d -= 1; s -= 1; *d = *s; }
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
		if(CPUFLagsHasBeenChecked == FALSE)
		{
			CPUFLagsHasBeenChecked = TRUE;
			CPUFlags = GetCpuFlags();
		}

		u8 *d = (u8 *)(Dest);

		if(CPUFlags & CPU_AVX2)
		{
			__m256i Fill32 = _mm256_set1_epi8((char)Value);
			while(Size >= 32)
			{
				_mm256_storeu_si256((__m256i *)d, Fill32);
				d += 32;
				Size -= 32;
			}
		}

		if(CPUFlags & CPU_SSE2)
		{
			if(Size >= 16)
			{
				__m128i Fill16 = _mm_set1_epi8((char)Value);
				while(Size >= 16)
				{
					_mm_storeu_si128((__m128i *)d, Fill16);
					d += 16;
					Size -= 16;
				}
			}
		}

		if(Size >= 8)
		{
			u64 Fill64 = 0x0101010101010101ULL * Value;
			*(u64 *)d = Fill64;
			d += 8;
			Size -= 8;
		}

		if(Size >= 4)
		{
			u32 Fill32Scalar = 0x01010101U * Value;
			*(u32 *)d = Fill32Scalar;
			d += 4;
			Size -= 4;
		}

		if(Size >= 2)
		{
			u16 Fill16Scalar = (u16)((Value << 8) | Value);
			*(u16 *)d = Fill16Scalar;
			d += 2;
			Size -= 2;
		}

		if(Size >= 1)
		{
			*d = Value;
		}
	}

	static inline u32 GetFirstMismatchIndex(u32 Mask) 
	{
		unsigned long Index;
		_BitScanForward(&Index, ~Mask);
		return (u32)Index;
	}

	/// <summary>
	/// Compares memory
	/// </summary>
	/// <param name="Buf1">Memory address</param>
	/// <param name="Buf2">Memory address to check against</param>
	/// <param name="Size">Size of bytes to check</param>
	/// <returns>0 if the buffers are equal, -1 if the first mismatch byte in Buf1 is less than Buf2, or 1 if the first mismatch byte in Buf1 is greater than Buf2</returns>
	i32 Memcmp(const void *Buf1, const void *Buf2, size_t Size)
	{
		if(CPUFLagsHasBeenChecked == FALSE)
		{
			CPUFLagsHasBeenChecked = TRUE;
			CPUFlags = GetCpuFlags();
		}
		
		const u8 *p1 = (const u8 *)Buf1;
		const u8 *p2 = (const u8 *)Buf2;

		if(CPUFlags & CPU_AVX2)
		{
			while(Size >= 32)
			{
				__m256i v1 = _mm256_loadu_si256((const __m256i *)p1);
				__m256i v2 = _mm256_loadu_si256((const __m256i *)p2);
				u32 mask = (u32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(v1, v2));

				if(mask != 0xFFFFFFFF)
				{
					u32 idx = GetFirstMismatchIndex(mask);
					return (p1[idx] < p2[idx]) ? -1 : 1;
				}
				p1 += 32; p2 += 32; Size -= 32;
			}
		}

		if(CPUFlags & CPU_SSE2)
		{
			while(Size >= 16)
			{
				__m128i v1 = _mm_loadu_si128((const __m128i *)p1);
				__m128i v2 = _mm_loadu_si128((const __m128i *)p2);
				u32 mask = (u32)_mm_movemask_epi8(_mm_cmpeq_epi8(v1, v2)) & 0xFFFF;

				if(mask != 0xFFFF)
				{
					u32 idx = GetFirstMismatchIndex(mask);
					return (p1[idx] < p2[idx]) ? -1 : 1;
				}
				p1 += 16; p2 += 16; Size -= 16;
			}
		}

		if(Size >= 8)
		{
			if(*(u64 *)p1 != *(u64 *)p2) goto tail_mismatch;
			p1 += 8; p2 += 8; Size -= 8;
		}

		if(Size >= 4)
		{
			if(*(u32 *)p1 != *(u32 *)p2) goto tail_mismatch;
			p1 += 4; p2 += 4; Size -= 4;
		}

		if(Size >= 2)
		{
			if(*(u16 *)p1 != *(u16 *)p2) goto tail_mismatch;
			p1 += 2; p2 += 2; Size -= 2;
		}

		if(Size >= 1)
		{
			if(*p1 != *p2) goto tail_mismatch;
		}

		return 0;

	tail_mismatch:
		while(Size > 0)
		{
			if(*p1 != *p2) return (*p1 < *p2) ? -1 : 1;
			p1++; p2++; Size--;
		}
		return 0;
	}
}