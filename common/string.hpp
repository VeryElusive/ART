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

	bool StringCompareCI(const char *String, const char *String2);
	bool StringCompareCI(const wchar_t *String, const wchar_t *String2);

	bool StringNCompareCI(const char *String, const char *String2, Size_t StringLength);
	bool StringNCompareCI(const wchar_t *String, const wchar_t *String2, Size_t StringLength);

	const char *StringFind(const char *String, const char *SubString);
	const char *StringNFind(const char *String, const char *SubString, Size_t SubStringLen);

	const wchar_t *StringFind(const wchar_t *String, const wchar_t *SubString);
	const wchar_t *StringNFind(const wchar_t *String, const wchar_t *SubString, Size_t SubStringLen);

	const char *StringFindCI(const char *String, const char *SubString);
	const char *StringNFindCI(const char *String, const char *SubString, Size_t SubStringLen);

	const wchar_t *StringFindCI(const wchar_t *String, const wchar_t *SubString);
	const wchar_t *StringNFindCI(const wchar_t *String, const wchar_t *SubString, Size_t SubStringLen);

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
