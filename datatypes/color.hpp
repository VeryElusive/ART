#pragma once

#include "../common/decl.hpp"
#include "../common/math.hpp"

namespace ART
{
	class Color_t
	{
	public:
		Color_t(u32 color = 0x000000ff) :
			r((float)((color >> 24) & 0xFF)), g((float)((color >> 16) & 0xFF)), b((float)((color >> 8) & 0xFF)), a((float)(color & 0xFF)) {}

		Color_t(int r, int g, int b, int a = 255) :
			r((float)r), g((float)g), b((float)b), a((float)a) {}

		Color_t(int r, int g, int b, float a) :
			r((float)r), g((float)g), b((float)b), a(255.f * a) {}

		Color_t(float r, float g, float b, float a = 255.f) :
			r(r), g(g), b(b), a(a) {}

#if 0
		Color_t(float Col[4]) :
			r(Col[0]), g(Col[1]), b(Col[2]), a(Col[3]) {}
#endif

		Color_t(u8 r, u8 g, u8 b, u8 a = 255) :
			r((float)r), g((float)g), b((float)b), a((float)a) {}

		Color_t ScaleAlpha(float Alpha) const
		{
			Color_t ret = *this;
			ret.a = (u8)(((float)ret.a) * Alpha);
			return ret;
		}

		Color_t Lerp(const Color_t &To, float t) const
		{
			if(r == To.r && g == To.g && b == To.b && a == To.a)
			{
				return *this;
			}

			t = ART::Min(t, 1.f);

			Color_t ret(
				ART::Clamp(ART::Lerp(t, (float)r, (float)To.r), 0.f, 255.f),
				ART::Clamp(ART::Lerp(t, (float)g, (float)To.g), 0.f, 255.f),
				ART::Clamp(ART::Lerp(t, (float)b, (float)To.b), 0.f, 255.f),
				ART::Clamp(ART::Lerp(t, (float)a, (float)To.a), 0.f, 255.f)
			);
			return ret;
		}

		float GetBrightness() const
		{
			// Luminance formula: 0.299*R + 0.587*G + 0.114*B
			return (0.299f * (float)r + 0.587f * (float)g + 0.114f * (float)b) / 255.f;
		}

		Color_t &operator=(u32 color)
		{
			r = (color >> 24) & 0xFF;
			g = (color >> 16) & 0xFF;
			b = (color >> 8) & 0xFF;
			a = color & 0xFF;
			return *this;
		}

		Color_t &operator=(Color_t color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
			return *this;
		}

		bool operator==(Color_t color)
		{
			return color.r == this->r
				&& color.g == this->g
				&& color.b == this->b
				&& color.a == this->a;
		}

		bool operator!=(Color_t color)
		{
			return color.r != this->r
				|| color.g != this->g
				|| color.b != this->b
				|| color.a != this->a;
		}

		float r, g, b, a = 255.f;
	};

}
