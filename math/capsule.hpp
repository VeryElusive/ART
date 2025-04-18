#pragma once

#include "../common/math.hpp"
#include "../datatypes/vector.hpp"

namespace ART
{
	inline float IntersectCapsule(ART::Vec3_t s1, ART::Vec3_t s2, ART::Vec3_t k1, ART::Vec3_t k2)
	{
		const auto u = s2 - s1;
		const auto v = k2 - k1;
		const auto w = s1 - k1;

		const auto a = u.Dot(u);
		const auto b = u.Dot(v);
		const auto c = v.Dot(v);
		const auto d = u.Dot(w);
		const auto e = v.Dot(w);
		const auto D = a * c - b * b;
		float sn, sd = D;
		float tn, td = D;

		if(D < EPSILON)
		{
			sn = 0.f;
			sd = 1.f;
			tn = e;
			td = c;
		}
		else
		{
			sn = b * e - c * d;
			tn = a * e - b * d;

			if(sn < 0.f)
			{
				sn = 0.f;
				tn = e;
				td = c;
			}
			else if(sn > sd)
			{
				sn = sd;
				tn = e + b;
				td = c;
			}
		}

		if(tn < 0.f)
		{
			tn = 0.f;

			if(-d < 0.f)
			{
				sn = 0.f;
			}
			else if(-d > a)
			{
				sn = sd;
			}
			else
			{
				sn = -d;
				sd = a;
			}
		}
		else if(tn > td)
		{
			tn = td;

			if(-d + b < 0.f)
			{
				sn = 0.f;
			}
			else if(-d + b > a)
			{
				sn = sd;
			}
			else
			{
				sn = -d + b;
				sd = a;
			}
		}

		const float sc = ART::AbsF(sn) < EPSILON ? 0.f : sn / sd;
		const float tc = ART::AbsF(tn) < EPSILON ? 0.f : tn / td;

		const auto dp = w + u * sc - v * tc;
		return dp.Length();
	}
}