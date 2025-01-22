#pragma once

#include "vector.hpp"

namespace ART
{
	struct Quaternion_t
	{
		float x, y, z, w;

		Quaternion_t(float _x, float _y, float _z, float _w) :
			w(_w), x(_x), y(_y), z(_z) {}

		Quaternion_t(Vec3_t vec) :
			w(0.f), x(vec.x), y(vec.y), z(vec.z) {}

		Vec3_t ToVector() const
		{
			return Vec3_t(x, y, z);
		}

		Quaternion_t Conjugate() const
		{
			return Quaternion_t(-x, -y, -z, w);
		}

		Quaternion_t operator*(const Quaternion_t &rhs) const
		{
			return Quaternion_t(
				w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
				w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
			);
		}

		Vec3_t Rotate(const Vec3_t v) const
		{
			const float tx = 2.f * (y * v.z - z * v.y);
			const float ty = 2.f * (z * v.x - x * v.z);
			const float tz = 2.f * (x * v.y - y * v.x);

			Vec3_t ret;

			// v + w t + q x t
			ret.x = v.x + w * tx + y * tz - z * ty;
			ret.y = v.y + w * ty + z * tx - x * tz;
			ret.z = v.z + w * tz + x * ty - y * tx;

			return ret;
		}
	};
}