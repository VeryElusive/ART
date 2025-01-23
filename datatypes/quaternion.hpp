#pragma once

#include "vector.hpp"

namespace ART
{
	struct Quaternion_t
	{
		float X, Y, Z, W;

		Quaternion_t(float _x, float _y, float _z, float _w) :
			W(_w), X(_x), Y(_y), Z(_z) {}

		Quaternion_t(Vec3_t vec) :
			W(0.f), X(vec.X), Y(vec.Y), Z(vec.Z) {}

		Vec3_t ToVector() const
		{
			return Vec3_t(X, Y, Z);
		}

		Quaternion_t Conjugate() const
		{
			return Quaternion_t(-X, -Y, -Z, W);
		}

		Quaternion_t operator*(const Quaternion_t &rhs) const
		{
			return Quaternion_t(
				W * rhs.X + X * rhs.W + Y * rhs.Z - Z * rhs.Y,
				W * rhs.Y - X * rhs.Z + Y * rhs.W + Z * rhs.X,
				W * rhs.Z + X * rhs.Y - Y * rhs.X + Z * rhs.W,
				W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z
			);
		}

		Vec3_t Rotate(const Vec3_t v) const
		{
			const float tx = 2.f * (Y * v.Z - Z * v.Y);
			const float ty = 2.f * (Z * v.X - X * v.Z);
			const float tz = 2.f * (X * v.Y - Y * v.X);

			Vec3_t ret;

			// v + w t + q X t
			ret.X = v.X + W * tx + Y * tz - Z * ty;
			ret.Y = v.Y + W * ty + Z * tx - X * tz;
			ret.Z = v.Z + W * tz + X * ty - Y * tx;

			return ret;
		}
	};
}