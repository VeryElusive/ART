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

	// remember to free after use!
	const char *StringConcat(const char *String1, const char *String2);
	const wchar_t *StringConcat(const wchar_t *String1, const wchar_t *String2);

	const char *StringNConcat(const char *String1, const char *String2, Size_t String1Length, Size_t String2Length);
	const wchar_t *StringNConcat(const wchar_t *String1, const wchar_t *String2, Size_t String1Length, Size_t String2Length);

	void IntegerToString(i32 number, char *Buf, Size_t BufSize);
	void FloatToString(float number, Size_t precision, char *Buf, Size_t BufSize);


	char ToLower(char Character);
	char ToUpper(char Character);

	void ToLower(char *Characters);
	void ToUpper(char *Characters);
}
