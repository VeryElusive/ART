#pragma once

#include "../common/decl.hpp"
#include "../common/math.hpp"

namespace ART
{
	class Color32_t;

	class Color_t
	{
	public:
		constexpr Color_t(u32 color = 0x000000ff) :
			r((float)((color >> 24) & 0xFF)), g((float)((color >> 16) & 0xFF)), b((float)((color >> 8) & 0xFF)), a((float)(color & 0xFF)) {}

		constexpr Color_t(int r, int g, int b, int a = 255) :
			r((float)r), g((float)g), b((float)b), a((float)a) {}

		constexpr Color_t(int r, int g, int b, float a) :
			r((float)r), g((float)g), b((float)b), a(255.f * a) {}

		constexpr Color_t(float r, float g, float b, float a = 255.f) :
			r(r), g(g), b(b), a(a) {}

		Color_t(const Color32_t &Color32);

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

		Color_t Brighten(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			Color_t ret = *this;
			ret.r = ART::Clamp(ret.r + (255.f - ret.r) * Amount, 0.f, 255.f);
			ret.g = ART::Clamp(ret.g + (255.f - ret.g) * Amount, 0.f, 255.f);
			ret.b = ART::Clamp(ret.b + (255.f - ret.b) * Amount, 0.f, 255.f);
			return ret;
		}

		Color_t Darken(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			Color_t ret = *this;
			ret.r = ART::Clamp(ret.r * (1.f - Amount), 0.f, 255.f);
			ret.g = ART::Clamp(ret.g * (1.f - Amount), 0.f, 255.f);
			ret.b = ART::Clamp(ret.b * (1.f - Amount), 0.f, 255.f);
			return ret;
		}

		Color_t Saturate(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			const float Lum = GetBrightness() * 255.f;
			const float Scale = 1.f + Amount;

			Color_t ret = *this;
			ret.r = ART::Clamp(Lum + (ret.r - Lum) * Scale, 0.f, 255.f);
			ret.g = ART::Clamp(Lum + (ret.g - Lum) * Scale, 0.f, 255.f);
			ret.b = ART::Clamp(Lum + (ret.b - Lum) * Scale, 0.f, 255.f);
			return ret;
		}

		Color_t Desaturate(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			const float Lum = GetBrightness() * 255.f;

			Color_t ret = *this;
			ret.r = ART::Clamp(ART::Lerp(Amount, ret.r, Lum), 0.f, 255.f);
			ret.g = ART::Clamp(ART::Lerp(Amount, ret.g, Lum), 0.f, 255.f);
			ret.b = ART::Clamp(ART::Lerp(Amount, ret.b, Lum), 0.f, 255.f);
			return ret;
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

	class Color32_t
	{
	public:
		constexpr Color32_t(u32 color = 0x000000ff) :
			r((color >> 24) & 0xFF), g((color >> 16) & 0xFF), b((color >> 8) & 0xFF), a(color & 0xFF) {}

		constexpr Color32_t(int r, int g, int b, int a = 255) :
			r(r), g(g), b(b), a(a) {}

		constexpr Color32_t(int r, int g, int b, float a) :
			r(r), g(g), b(b), a((u8)(int)(255.f * a)) {}

		constexpr Color32_t(float r, float g, float b, float a = 1.f) :
			r((u8)(u32)(r * 255.f)), g((u8)(u32)(g * 255.f)), b((u8)(u32)(b * 255.f)), a((u8)(u32)(a * 255.f)) {}

		constexpr Color32_t(float Col[4]) :
			r((u8)(u32)(Col[0] * 255.f)), g((u8)(u32)(Col[1] * 255.f)), b((u8)(u32)(Col[2] * 255.f)), a((u8)(u32)(Col[3] * 255.f)) {}

		constexpr Color32_t(u8 r, u8 g, u8 b, u8 a = 255) :
			r(r), g(g), b(b), a(a) {}

		constexpr Color32_t(Color_t Col) :
			r((u8)(u32)Col.r), g((u8)(u32)Col.g), b((u8)(u32)Col.b), a((u8)(u32)Col.a) {}

		Color32_t ScaleAlpha(float Alpha) const
		{
			Color32_t ret = *this;
			ret.a = (u8)(((float)ret.a) * Alpha);
			return ret;
		}

		Color32_t Lerp(const Color32_t &To, float t) const
		{
			if(r == To.r && g == To.g && b == To.b && a == To.a)
			{
				return *this;
			}

			t = ART::Min(t, 1.f);

			Color32_t ret(
				(u8)ART::Clamp(ART::Lerp(t, (float)r, (float)To.r), 0.f, 255.f),
				(u8)ART::Clamp(ART::Lerp(t, (float)g, (float)To.g), 0.f, 255.f),
				(u8)ART::Clamp(ART::Lerp(t, (float)b, (float)To.b), 0.f, 255.f),
				(u8)ART::Clamp(ART::Lerp(t, (float)a, (float)To.a), 0.f, 255.f)
			);
			return ret;
		}

		Color32_t Brighten(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			Color32_t ret = *this;
			ret.r = (u8)ART::Clamp((float)ret.r + (255.f - (float)ret.r) * Amount, 0.f, 255.f);
			ret.g = (u8)ART::Clamp((float)ret.g + (255.f - (float)ret.g) * Amount, 0.f, 255.f);
			ret.b = (u8)ART::Clamp((float)ret.b + (255.f - (float)ret.b) * Amount, 0.f, 255.f);
			return ret;
		}

		Color32_t Darken(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			Color32_t ret = *this;
			ret.r = (u8)ART::Clamp((float)ret.r * (1.f - Amount), 0.f, 255.f);
			ret.g = (u8)ART::Clamp((float)ret.g * (1.f - Amount), 0.f, 255.f);
			ret.b = (u8)ART::Clamp((float)ret.b * (1.f - Amount), 0.f, 255.f);
			return ret;
		}

		Color32_t Saturate(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			const float Lum = GetBrightness() * 255.f;
			const float Scale = 1.f + Amount;

			Color32_t ret = *this;
			ret.r = (u8)ART::Clamp(Lum + ((float)ret.r - Lum) * Scale, 0.f, 255.f);
			ret.g = (u8)ART::Clamp(Lum + ((float)ret.g - Lum) * Scale, 0.f, 255.f);
			ret.b = (u8)ART::Clamp(Lum + ((float)ret.b - Lum) * Scale, 0.f, 255.f);
			return ret;
		}

		Color32_t Desaturate(float Amount) const
		{
			Amount = ART::Clamp(Amount, 0.f, 1.f);

			const float Lum = GetBrightness() * 255.f;

			Color32_t ret = *this;
			ret.r = (u8)ART::Clamp(ART::Lerp(Amount, (float)ret.r, Lum), 0.f, 255.f);
			ret.g = (u8)ART::Clamp(ART::Lerp(Amount, (float)ret.g, Lum), 0.f, 255.f);
			ret.b = (u8)ART::Clamp(ART::Lerp(Amount, (float)ret.b, Lum), 0.f, 255.f);
			return ret;
		}

		float GetBrightness() const
		{
			// Luminance formula: 0.299*R + 0.587*G + 0.114*B
			return (0.299f * (float)r + 0.587f * (float)g + 0.114f * (float)b) / 255.f;
		}

		Color32_t &operator=(u32 color)
		{
			r = (color >> 24) & 0xFF;
			g = (color >> 16) & 0xFF;
			b = (color >> 8) & 0xFF;
			a = color & 0xFF;
			return *this;
		}

		Color32_t &operator=(Color32_t color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
			return *this;
		}

		bool operator==(Color32_t color)
		{
			return color.r == this->r
				&& color.g == this->g
				&& color.b == this->b
				&& color.a == this->a;
		}

		bool operator!=(Color32_t color)
		{
			return color.r != this->r
				|| color.g != this->g
				|| color.b != this->b
				|| color.a != this->a;
		}

		u8 r, g, b, a = 255;
	};

	inline Color_t::Color_t(const Color32_t &Color32)
		: r((float)Color32.r),
		g((float)Color32.g),
		b((float)Color32.b),
		a((float)Color32.a) {}
}
