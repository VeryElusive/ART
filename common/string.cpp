#include "string.hpp"
#include "memory.hpp"
#include "heap.hpp"

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

const char *ART::StringFind(const char *String, const char *SubString)
{
	Size_t SubStringLen = ART::StringLength(SubString);
	return StringNFind(String, SubString, SubStringLen);
}

const char *ART::StringNFind(const char *String, const char *SubString, Size_t SubStringLen)
{
	if(!String || !SubString)
	{
		return NULL;
	}

	if(SubStringLen == 0)
	{
		return String;
	}

	Size_t StringLen = ART::StringLength(String);
	if(StringLen < SubStringLen)
	{
		return NULL;
	}

	for(int i = 0; i <= StringLen - SubStringLen; i++)
	{
		bool Found = TRUE;

		for(int j = 0; j < SubStringLen; j++)
		{
			if(String[i + j] != SubString[j])
			{
				Found = FALSE;
				break;
			}
		}

		if(Found)
		{
			return String + i;
		}
	}

	return NULL;
}

const wchar_t *ART::StringFind(const wchar_t *String, const wchar_t *SubString)
{
	Size_t SubStringLen = ART::StringLength(SubString);
	return StringNFind(String, SubString, SubStringLen);
}

const wchar_t *ART::StringNFind(const wchar_t *String, const wchar_t *SubString, Size_t SubStringLen)
{
	if(!String || !SubString)
	{
		return NULL;
	}

	if(SubStringLen == 0)
	{
		return String;
	}

	Size_t StringLen = ART::StringLength(String);
	if(StringLen < SubStringLen)
	{
		return NULL;
	}

	for(int i = 0; i <= StringLen - SubStringLen; i++)
	{
		bool Found = TRUE;

		for(int j = 0; j < SubStringLen; j++)
		{
			if(String[i + j] != SubString[j])
			{
				Found = FALSE;
				break;
			}
		}

		if(Found)
		{
			return String + i;
		}
	}

	return NULL;
}

const char *ART::StringFindCI(const char *String, const char *SubString)
{
	Size_t SubStringLen = ART::StringLength(SubString);
	return StringNFindCI(String, SubString, SubStringLen);
}

const char *ART::StringNFindCI(const char *String, const char *SubString, Size_t SubStringLen)
{
	if(!String || !SubString)
	{
		return NULL;
	}

	if(SubStringLen == 0)
	{
		return String;
	}

	Size_t StringLen = ART::StringLength(String);
	if(StringLen < SubStringLen)
	{
		return NULL;
	}

	for(int i = 0; i <= StringLen - SubStringLen; i++)
	{
		bool Found = TRUE;

		for(int j = 0; j < SubStringLen; j++)
		{
			if(ART::ToLower(String[i + j]) != ART::ToLower(SubString[j]))
			{
				Found = FALSE;
				break;
			}
		}

		if(Found)
		{
			return String + i;
		}
	}

	return NULL;
}

const wchar_t *ART::StringFindCI(const wchar_t *String, const wchar_t *SubString)
{
	Size_t SubStringLen = ART::StringLength(SubString);
	return StringNFindCI(String, SubString, SubStringLen);
}

const wchar_t *ART::StringNFindCI(const wchar_t *String, const wchar_t *SubString, Size_t SubStringLen)
{
	if(!String || !SubString)
	{
		return NULL;
	}

	if(SubStringLen == 0)
	{
		return String;
	}

	Size_t StringLen = ART::StringLength(String);
	if(StringLen < SubStringLen)
	{
		return NULL;
	}

	for(int i = 0; i <= StringLen - SubStringLen; i++)
	{
		bool Found = TRUE;

		for(int j = 0; j < SubStringLen; j++)
		{
			if(ART::ToLower(String[i + j]) != ART::ToLower(SubString[j]))
			{
				Found = FALSE;
				break;
			}
		}

		if(Found)
		{
			return String + i;
		}
	}

	return NULL;
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

bool ART::StringCompareCI(const char *String, const char *String2)
{
	return StringNCompareCI(String, String2, StringLength(String));
}

bool ART::StringNCompareCI(const char *String, const char *String2, Size_t StringLength)
{
	for(Size_t i = 0; i < StringLength; ++i)
	{
		char C1 = String[i];
		char C2 = String2[i];

		if(C1 >= 'A' && C1 <= 'Z') C1 += 32;
		if(C2 >= 'A' && C2 <= 'Z') C2 += 32;

		if(C1 != C2) return false;
	}

	return true;
}

bool ART::StringCompareCI(const wchar_t *String, const wchar_t *String2)
{
	return StringNCompareCI(String, String2, StringLength(String));
}

bool ART::StringNCompareCI(const wchar_t *String, const wchar_t *String2, Size_t StringLength)
{
	for(Size_t i = 0; i < StringLength; ++i)
	{
		wchar_t C1 = String[i];
		wchar_t C2 = String2[i];

		if(C1 >= L'A' && C1 <= L'Z') C1 += 32;
		if(C2 >= L'A' && C2 <= L'Z') C2 += 32;

		if(C1 != C2) return false;
	}

	return true;
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

void ART::ToLower(char *Characters)
{
	for(char *Current = Characters; *Current != '\0'; Current++)
	{
		*Current = ToLower(*Current);
	}
}

void ART::ToUpper(char *Characters)
{
	for(char *Current = Characters; *Current != '\0'; Current++)
	{
		*Current = ToUpper(*Current);
	}
}

const char *ART::StringConcat(const char *String1, const char *String2)
{
	return ART::StringNConcat(String1, String2, ART::StringLength(String1), ART::StringLength(String2));
}

const wchar_t *ART::StringConcat(const wchar_t *String1, const wchar_t *String2)
{
	return ART::StringNConcat(String1, String2, ART::StringLength(String1), ART::StringLength(String2));
}

const char *ART::StringNConcat(const char *String1, const char *String2, Size_t String1Length, Size_t String2Length)
{
	char *Out = (char *)ART::Alloc((String1Length + String2Length + 1) * sizeof(const char));

	ART::Memcpy((void *)Out, String1, String1Length * sizeof(const char));
	ART::Memcpy((void *)&Out[String1Length], String2, String2Length * sizeof(const char));

	Out[String1Length + String2Length] = '\0';

	return (const char *)Out;
}

const wchar_t *ART::StringNConcat(const wchar_t *String1, const wchar_t *String2, Size_t String1Length, Size_t String2Length)
{
	wchar_t *Out = (wchar_t *)ART::Alloc((String1Length + String2Length + 1) * sizeof(const wchar_t));

	ART::Memcpy((void *)Out, String1, String1Length * sizeof(const wchar_t));
	ART::Memcpy((void *)&Out[String1Length], String2, String2Length * sizeof(const wchar_t));

	Out[String1Length + String2Length] = '\0';

	return (const wchar_t *)Out;
}

void ART::IntegerToString(i32 number, char *Buf, Size_t BufSize)
{
	bool IsNegative = (number < 0);
	u32 AbsNumber = IsNegative ? -(u32)(number) : number;
	char *Ptr = Buf + BufSize - 1;
	*Ptr = '\0';

	do {
		*--Ptr = '0' + (AbsNumber % 10);
		AbsNumber /= 10;
	} while(AbsNumber > 0);

	if(IsNegative) {
		*--Ptr = '-';
	}

	Size_t Length = Buf + BufSize - Ptr - 1;
	for(Size_t i = 0; i < Length; ++i)
	{
		Buf[i] = Ptr[i];
	}
	Buf[Length] = '\0';
}

void ART::FloatToString(float number, Size_t precision, char *Buf, Size_t BufSize)
{
	char TempBuffer[32];
	IntegerToString((i32)number, TempBuffer, sizeof(TempBuffer));

	Size_t IntPartLength = ART::StringLength(TempBuffer);
	char *Ptr = Buf;

	for(Size_t i = 0; i < IntPartLength && (Ptr - Buf) < BufSize - 1; ++i)
	{
		*Ptr++ = TempBuffer[i];
	}

	if((Ptr - Buf) < BufSize - 1)
	{
		*Ptr++ = '.';
	}

	float FractionPart = number - (i32)number;
	if(FractionPart < 0)
	{
		FractionPart = -FractionPart;
	}

	for(Size_t i = 0; i < precision && (Ptr - Buf) < BufSize - 1; ++i)
	{
		FractionPart *= 10;
		int Digit = (int)FractionPart % 10;
		*Ptr++ = '0' + Digit;
	}
	*Ptr = '\0';
}