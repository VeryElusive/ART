#include "luhash.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

consteval u32 ART::Hash::LUHash::Hash2Const(const void *Data, u32 Seed, Size_t Length)
{
	const u8 *_Data = (u8 *)(Data);
	u32         Hash = 'LUTL' ^ Seed;

	for(Size_t Index = 0; Length != 0;)
	{
		/*if(Length >= 8)
		{
			Hash = _mm_crc32_u64(Hash, *(libutil_u64 *)(&_Data[Index]));

			Index += 8;
			Length -= 8;
		}
		else */if(Length >= 4)
		{
			Hash = _mm_crc32_u32(Hash, *(u32 *)(&_Data[Index]));

			Index += 4;
			Length -= 4;
		}
		else if(Length >= 2)
		{
			Hash = _mm_crc32_u16(Hash, *(u16 *)(&_Data[Index]));

			Index += 2;
			Length -= 2;
		}
		else
		{
			Hash = _mm_crc32_u8(Hash, _Data[Index]);

			++Index;
			--Length;
		}
	}

	return Hash ^ 'LUTL';
}