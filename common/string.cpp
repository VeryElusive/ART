#include "string.hpp"
#include "memory.hpp"

#include "math.hpp"

Size_t ART::StringLength(const char *String)
{
	return StringNLength(String, INT_MAX);
}

Size_t ART::StringNLength(const char *String, Size_t MaxLength)
{
	Size_t Length = 0;
	for(; *String != '\0' && MaxLength > 0; ++String, ++Length, --MaxLength) {}

	return Length;
}

Size_t ART::StringLength(const wchar_t *String)
{
	return StringNLength(String, INT_MAX);
}

Size_t ART::StringNLength(const wchar_t *String, Size_t MaxLength)
{
	Size_t Length = 0;
	for(; *String != '\0' && MaxLength > 0; ++String, ++Length, --MaxLength) {}

	return Length;
}


bool ART::StringCompare(const char *String, const char *String2)
{
	return StringNCompare(String, String2, StringLength(String));
}

bool ART::StringNCompare(const char *String, const char *String2, Size_t StringLength)
{
	return (ART::Memcmp(String, String2, StringLength) == 0);
}

bool ART::StringCompare(const wchar_t *String, const wchar_t *String2)
{
	return StringNCompare(String, String2, StringLength(String));
}

bool ART::StringNCompare(const wchar_t *String, const wchar_t *String2, Size_t StringLength)
{
	return (ART::Memcmp(String, String2, StringLength) == 0);
}

char ART::ToLower(char Character)
{
	if(Character >= 'A' && Character <= 'Z') 
	{
		return Character - 'A' + 'a';
	}
	return Character;
}

char ART::ToUpper(char Character)
{
	if(Character >= 'a' && Character <= 'z') 
	{
		return Character - 'a' + 'A';
	}

	return Character;
}

void ART::ToLower(char *Character)
{
	for(char *Current = Character; *Current != '\0'; Current++)
	{
		*Current = ToLower(*Current);
	}
}

void ART::ToUpper(char *Character)
{
	for(char *Current = Character; *Current != '\0'; Current++)
	{
		*Current = ToUpper(*Current);
	}
}