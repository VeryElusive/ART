#pragma once

#ifndef ART_ALREADY_DEFINED

#include "../common/decl.hpp"
#include "../common/math.hpp"

namespace ART
{
	class Color
	{
	public:
		Color(u32 color = 0x000000ff) :
			r((color >> 24) & 0xFF), g((color >> 16) & 0xFF), b((color >> 8) & 0xFF), a(color & 0xFF) {}

		Color(int r, int g, int b, int a = 255) :
			r(r), g(g), b(b), a(a) {}

		Color(float r, float g, float b, float a = 1.f) :
			r((u8)(u32)(r * 255.f)), g((u8)(u32)(g * 255.f)), b((u8)(u32)(b * 255.f)), a((u8)(u32)(a * 255.f)) {}

		Color(float Col[4]) :
			r((u8)(u32)(Col[0] * 255.f)), g((u8)(u32)(Col[1] * 255.f)), b((u8)(u32)(Col[2] * 255.f)), a((u8)(u32)(Col[3] * 255.f)) {}

		Color(u8 r, u8 g, u8 b, u8 a = 255) :
			r(r), g(g), b(b), a(a) {}


		u32 ToUInt() const
		{
			return ((u32)(r) << 24) | ((u32)(g) << 16) | ((u32)(b) << 8) | (u32)(a);
		}

		Color ScaleAlpha(float Alpha) const
		{
			Color ret = *this;
			ret.a = (u8)(((float)ret.a) * Alpha);
			return ret;
		}

		Color Lerp(const Color &To, float t) const
		{
			if(r == To.r && g == To.g && b == To.b && a == To.a)
			{
				return *this;
			}

			t = ART::Math::Min(t, 1.f);

			Color ret(
				(u8)ART::Math::Clamp(ART::Math::Lerp(t, (float)r, (float)To.r), 0.f, 255.f),
				(u8)ART::Math::Clamp(ART::Math::Lerp(t, (float)g, (float)To.g), 0.f, 255.f),
				(u8)ART::Math::Clamp(ART::Math::Lerp(t, (float)b, (float)To.b), 0.f, 255.f),
				(u8)ART::Math::Clamp(ART::Math::Lerp(t, (float)a, (float)To.a), 0.f, 255.f)
			);
			return ret;
		}

		Color &operator=(u32 color)
		{
			r = (color >> 24) & 0xFF;
			g = (color >> 16) & 0xFF;
			b = (color >> 8) & 0xFF;
			a = color & 0xFF;
			return *this;
		}

		Color &operator=(Color color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
			return *this;
		}

		bool operator==(Color color)
		{
			return color.r == this->r
				&& color.g == this->g
				&& color.b == this->b
				&& color.a == this->a;
		}

		bool operator!=(Color color)
		{
			return color.r != this->r
				|| color.g != this->g
				|| color.b != this->b
				|| color.a != this->a;
		}

		u8 r, g, b, a = 255;
	};

}

#else
#include "ART/datatypes/color.hpp"
#endif