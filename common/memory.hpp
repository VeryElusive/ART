#pragma once

#include "../common/decl.hpp"

namespace ART
{
	void Memcpy(void *Dest, const void *Src, size_t Size);

	void Memmove(void *Dest, const void *Src, size_t Size);

	void Memset(void *Dest, u8 Value, size_t Size);

	i32 Memcmp(const void *Buf1, const void *Buf2, size_t Size);
}