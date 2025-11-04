#pragma once

#define NULL 0
#define FALSE 0
#define TRUE 1

typedef char			   i8;
typedef unsigned char      u8;
typedef short              i16;
typedef unsigned short     u16;
typedef int                i32;
typedef unsigned int       u32;
typedef __int64            i64;
typedef unsigned __int64   u64;

#ifdef ART_64BIT
typedef u64                Size_t;
typedef u64                Ptr_t;
#else
typedef u32                Size_t;
typedef u32                Ptr_t;
#endif


#define MB_LEN_MAX    5
#define SHRT_MIN    (-32768)
#define SHRT_MAX      32767
#define USHRT_MAX     0xffff
#define INT_MIN     (-2147483647 - 1)
#define INT_MAX       2147483647
#define UINT_MAX      0xffffffff
#define LONG_MIN    (-2147483647L - 1)
#define LONG_MAX      2147483647L
#define ULONG_MAX     0xffffffffUL
#define LLONG_MAX     9223372036854775807i64
#define LLONG_MIN   (-9223372036854775807i64 - 1)
#define ULLONG_MAX    0xffffffffffffffffui64

#ifdef ART_64BIT
#define UNSIGNED_MAX    ULLONG_MAX
#else
#define UNSIGNED_MAX      ULONG_MAX
#endif

#define _I8_MIN     (-127i8 - 1)
#define _I8_MAX       127i8
#define _UI8_MAX      0xffui8

#define _I16_MIN    (-32767i16 - 1)
#define _I16_MAX      32767i16
#define _UI16_MAX     0xffffui16

#define _I32_MIN    (-2147483647i32 - 1)
#define _I32_MAX      2147483647i32
#define _UI32_MAX     0xffffffffui32

#define _I64_MIN    (-9223372036854775807i64 - 1)
#define _I64_MAX      9223372036854775807i64
#define _UI64_MAX     0xffffffffffffffffui64


#define FLT_DECIMAL_DIG  9                       // # of decimal digits of rounding precision
#define FLT_DIG          6                       // # of decimal digits of precision
#define FLT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0
#define FLT_HAS_SUBNORM  1                       // type does support subnormal numbers
#define FLT_GUARD        0
#define FLT_MANT_DIG     24                      // # of bits in mantissa
#define FLT_MAX          3.402823466e+38F        // max value
#define FLT_MAX_10_EXP   38                      // max decimal exponent
#define FLT_MAX_EXP      128                     // max binary exponent
#define FLT_MIN          1.175494351e-38F        // min normalized positive value
#define FLT_MIN_10_EXP   (-37)                   // min decimal exponent
#define FLT_MIN_EXP      (-125)                  // min binary exponent
#define FLT_NORMALIZE    0
#define FLT_RADIX        2                       // exponent radix
#define FLT_TRUE_MIN     1.401298464e-45F        // min positive value

namespace ART
{
	inline bool IsFloatFinite(float X)
	{
		union
		{
			float F;
			unsigned int U;
		} Value = {X};

		unsigned int Exponent = (Value.U >> 23) & 0xFF;
		return Exponent != 0xFF;
	}
}
