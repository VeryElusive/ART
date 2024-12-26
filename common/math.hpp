#pragma once

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

#define HALF_PI                 1.5707963267948966f  // half pi
#define QUARTER_PI              0.7853981633974483f  // quarter pi
#define PI                      3.1415926535897932f  // pi const
#define PI_2                    6.2831853071795865f  // pi double
#define GOLDEN_RATIO            1.6180339887498f     // golden ratio
#define RADPI                   57.295779513082f     // pi in radians
#define EPSILON                 1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0

namespace ART
{
	namespace Math
	{
		template <typename T = int>
		inline T Min(T a, T b)
		{
			return (a > b) ? b : a;
		}

		template <typename T = int>
		inline T Max(T a, T b)
		{
			return (a > b) ? a : b;
		}

		template <typename T = int>
		inline T Clamp(T Value, T Minimum, T Maximum)
		{
			return Max<T>(Min<T>(Value, Maximum), Minimum);
		}
	}
}