#pragma once

#include "ART/common/decl.hpp"

namespace ART
{
	namespace LUHash
	{
		u32 Hash2(const char *Data, u32 Seed = 0, Size_t Length = 0);

		consteval u32 Hash2Const(const char *_Data, Size_t Length, u32 Seed)
		{
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
					u32 chunk = (_Data[Index + 3] << 24) | (_Data[Index + 2] << 16) |
						(_Data[Index + 1] << 8) | _Data[Index];
					Hash = CRC32Update(Hash, chunk & 0xFF);
					Hash = CRC32Update(Hash, (chunk >> 8) & 0xFF);
					Hash = CRC32Update(Hash, (chunk >> 16) & 0xFF);
					Hash = CRC32Update(Hash, (chunk >> 24) & 0xFF);
					Index += 4;
					Length -= 4;
				}
				else if(Length >= 2) 
				{
					u16 chunk = (_Data[Index + 1] << 8) | _Data[Index];
					Hash = CRC32Update(Hash, chunk & 0xFF);
					Hash = CRC32Update(Hash, (chunk >> 8) & 0xFF);
					Index += 2;
					Length -= 2;
				}
				else 
				{
					Hash = CRC32Update(Hash, _Data[Index]);
					++Index;
					--Length;
				}
			}
			return Hash ^ 'LUTL';
		}

		consteval u32 Hash2Const(const char *Data, u32 Seed = 0)
		{
			int Length = 0;
			do
			{
				Length++;
			} while(Data[Length] != '\0');

			return Hash2Const(Data, Length, Seed);
		}
	}
}
