#pragma once

#include "decl.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#endif

#include <xmmintrin.h>

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
#define DOUBLE_PI               6.2831853071795865f  // pi double
#define GOLDEN_RATIO            1.6180339887498f     // golden ratio
#define RADPI                   57.295779513082f     // pi in radians
#define EPSILON                 1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0

namespace ART
{
	template <typename T = i32>
	inline T Min(T a, T b)
	{
		return (a > b) ? b : a;
	}

	template <typename T = i32>
	inline T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template <typename T = i32>
	inline T Clamp(T Value, T Minimum, T Maximum)
	{
		return Max<T>(Min<T>(Value, Maximum), Minimum);
	}

	inline float AbsF(float Value)
	{
		*(i32 *)(&Value) &= ~(0x80000000);

		return Value;
	}

	inline i32 Abs(i32 Value)
	{
		Value &= ~(0x80000000);
		return Value;
	}

	template <typename T = float>
	T Interpolate(float Factor, T X, T Y)
	{
		if(X == Y)
		{
			return X;
		}

		Factor = Clamp<float>(Factor, 0.f, 1.f);

		return (T)(Y * Factor + X * (1.f - Factor));
	}

	template <typename T = float>
	inline T Lerp(float Factor, T X, T Y)
	{
		if(X == Y)
		{
			return X;
		}

		Factor = Clamp<float>(Factor, 0.f, 1.f);

		return (T)(X + (Factor * (Y - X)));
	}

	/* intrin */
	inline float Cos(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_cos_ps(_mm_load1_ps(&Value))
		);

		return Out;
	}

	inline float Sin(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_sin_ps(_mm_load1_ps(&Value))
		);

		return Out;
	}

	inline float Acos(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_acos_ps(_mm_load1_ps(&Value))
		);

		return Out;
	}

	inline float Sqrt(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_sqrt_ss(_mm_load_ss(&Value))
		);

		return Out;
	}

	inline float Rsqrt(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_rsqrt_ss(_mm_load_ss(&Value))
		);

		return Out;
	}

	inline float InvSqrt(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_invsqrt_ps(_mm_load_ps1(&Value))
		);

		return Out;
	}

	inline float Pow(float Value, float Power)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_pow_ps(_mm_load_ps1(&Value), _mm_load_ps1(&Power))
		);

		return Out;
	}

	inline float Mod(float X, float Y)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_fmod_ps(_mm_load_ss(&X), _mm_load_ss(&Y))
		);

		return Out;
	}

	float Ceil(float Value)
	{
		float Out;
		__m128 In = _mm_load_ss(&Value);
		_mm_store_ss(
			&Out,
			_mm_round_ss(In, In, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)
		);

		return Out;
	}

	float Floor(float Value)
	{
		float Out;
		__m128 In = _mm_load_ss(&Value);
		_mm_store_ss(
			&Out,
			_mm_round_ss(In, In, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)
		);

		return Out;
	}

	float Round(float Value)
	{
		float Out;
		__m128 In = _mm_load_ss(&Value);
		_mm_store_ss(
			&Out,
			_mm_round_ss(In, In, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;)
		);

		return Out;
	}
}
