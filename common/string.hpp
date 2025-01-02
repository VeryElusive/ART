#pragma once

#include "decl.hpp"

namespace ART
{
	Size_t StringLength(const char *String);
	Size_t StringLength(const wchar_t *String);

	Size_t StringNLength(const char *String, Size_t MaxLength);
	Size_t StringNLength(const wchar_t *String, Size_t MaxLength);

	bool StringCompare(const char *String, const char *String2);
	bool StringCompare(const wchar_t *String, const wchar_t *String2);

	bool StringNCompare(const char *String, const char *String2, Size_t StringLength);
	bool StringNCompare(const wchar_t *String, const wchar_t *String2, Size_t StringLength);


	char ToLower(char Character);
	char ToUpper(char Character);

	void ToLower(char *Character);
	void ToUpper(char *Character);
}
