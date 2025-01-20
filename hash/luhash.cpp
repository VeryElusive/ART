#include "luhash.hpp"

#include "../common/string.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

u32 ART::LUHash::Hash2(const char *Data, u32 Seed, Size_t Length)
{
	if(Length == 0)
	{
		Length = ART::StringLength(Data);
	}

	auto CRC32Update = [](u32 crc, u8 data) constexpr -> u32
		{
			constexpr u32 polynomial = 0x82F63B78U;  // CRC-32C polynomial
			crc ^= data;
			for(int i = 0; i < 8; ++i)
			{
				crc = (crc >> 1) ^ ((crc & 1) ? polynomial : 0);
			}
			return crc;
		};

	u32 Hash = 'LUTL' ^ Seed;

	for(Size_t Index = 0; Length != 0;)
	{
		if(Length >= 4)
		{
			u32 chunk = (Data[Index + 3] << 24) | (Data[Index + 2] << 16) |
				(Data[Index + 1] << 8) | Data[Index];
			Hash = CRC32Update(Hash, chunk & 0xFF);
			Hash = CRC32Update(Hash, (chunk >> 8) & 0xFF);
			Hash = CRC32Update(Hash, (chunk >> 16) & 0xFF);
			Hash = CRC32Update(Hash, (chunk >> 24) & 0xFF);
			Index += 4;
			Length -= 4;
		}
		else if(Length >= 2)
		{
			u16 chunk = (Data[Index + 1] << 8) | Data[Index];
			Hash = CRC32Update(Hash, chunk & 0xFF);
			Hash = CRC32Update(Hash, (chunk >> 8) & 0xFF);
			Index += 2;
			Length -= 2;
		}
		else
		{
			Hash = CRC32Update(Hash, Data[Index]);
			++Index;
			--Length;
		}
	}
	return Hash ^ 'LUTL';
}