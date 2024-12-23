#include "string.hpp"

#include "math.hpp"

Size_t ART::String::StringLength(const char *String)
{
	return StringNLength(String, INT_MAX);
}

Size_t ART::String::StringNLength(const char *String, Size_t MaxLength)
{
	Size_t Length = 0;
	for(; *String != '\0' && MaxLength > 0; ++String, ++Length, --MaxLength) {}

	return Length;
}

Size_t ART::String::StringLength(const wchar_t *String)
{
	return StringNLength(String, INT_MAX);
}

Size_t ART::String::StringNLength(const wchar_t *String, Size_t MaxLength)
{
	Size_t Length = 0;
	for(; *String != '\0' && MaxLength > 0; ++String, ++Length, --MaxLength) {}

	return Length;
}