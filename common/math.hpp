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

	inline float DegreeToRadians(float Degrees)
	{
		return Degrees * PI / 180.f;
	}

	inline float RadiansToDegree(float Radians)
	{
		return Radians * (180.0f / PI);
	}

	template <typename T = float>
	inline T Interpolate(float Factor, T X, T Y)
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

	inline float Asin(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_asin_ps(_mm_load1_ps(&Value))
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

	inline float Exp(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_exp_ps(_mm_load_ps1(&Value))
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

	inline float Ceil(float Value)
	{
		float Out;
		__m128 In = _mm_load_ss(&Value);
		_mm_store_ss(
			&Out,
			_mm_round_ss(In, In, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)
		);

		return Out;
	}

	inline float Floor(float Value)
	{
		float Out;
		__m128 In = _mm_load_ss(&Value);
		_mm_store_ss(
			&Out,
			_mm_round_ss(In, In, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)
		);

		return Out;
	}

	inline float Round(float Value)
	{
		float Out;
		__m128 In = _mm_load_ss(&Value);
		_mm_store_ss(
			&Out,
			_mm_round_ss(In, In, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)
		);

		return Out;
	}

	inline float Tan(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_tan_ps(_mm_load1_ps(&Value))
		);

		return Out;
	}

	inline float Atan(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_atan_ps(_mm_load1_ps(&Value))
		);

		return Out;
	}

	inline float Atan2(float Value, float Dividor)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_atan2_ps(_mm_load1_ps(&Value), _mm_load1_ps(&Dividor))
		);

		return Out;
	}

	
	inline float Tanh(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_tanh_ps(_mm_load1_ps(&Value))
		);

		return Out;
	}

	
	inline float Atanh(float Value)
	{
		float Out;
		_mm_store_ss(
			&Out,
			_mm_atanh_ps(_mm_load1_ps(&Value))
		);

		return Out;
	}

	/* Combinations */
	inline float AngleDifference(float Dst, float Src)
	{
		float Delta = Mod(Dst - Src, 360.f);

		if(Dst > Src && Delta >= 180.f)
		{
			Delta -= 360.f;
		}
		else if(Delta <= -180.f)
		{
			Delta += 360.f;
		}

		return Delta;
	}

	inline float NormalizeYaw(float Angle)
	{
		const float Revolutions = Round(AbsF(Angle / 360.f));
		if(Angle > 180.f || Angle < -180.f)
		{
			if(Angle < 0.f)
			{
				Angle = (Angle + 360.f * Revolutions);
			}
			else
			{
				Angle = (Angle - 360.f * Revolutions);
			}
			return Angle;
		}
		return Angle;
	}

	inline float NormalizePitch(float Angle)
	{
		return Clamp(Angle, -89.f, 89.f);
	}
}
