#pragma once

#include "decl.hpp"

namespace ART
{
	namespace String
	{
		Size_t StringLength(const char		*String);
		Size_t StringLength(const wchar_t	*String);

		Size_t StringNLength(const char *String, Size_t MaxLength);
		Size_t StringNLength(const wchar_t *String, Size_t MaxLength);
	}
}