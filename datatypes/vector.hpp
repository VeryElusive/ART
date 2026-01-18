#pragma once

#include "../common/decl.hpp"
#include "../common/math.hpp"

namespace ART
{
	struct Vec2_t
	{
		inline Vec2_t& operator /= (float k) {
			this->x /= k;
			this->y /= k;

			return *this;
		}

		inline Vec2_t& operator *= (float k) {
			this->x *= k;
			this->y *= k;
			return *this;
		}

		inline Vec2_t& operator -= (float k) {
			this->x -= k;
			this->y -= k;
			return *this;
		}

		inline Vec2_t& operator += (float k) {
			this->x += k;
			this->y += k;
			return *this;
		}

		inline Vec2_t operator / (float k) const {
			Vec2_t n = *this;
			n /= k;
			return n;
		}

		inline Vec2_t operator * (float k) const {
			Vec2_t n = *this;
			n *= k;
			return n;
		}

		inline Vec2_t operator - (float k) const {
			Vec2_t n = *this;
			n -= k;
			return n;
		}

		inline Vec2_t operator + (float k) const {
			Vec2_t n = *this;
			n += k;
			return n;
		}

		inline Vec2_t& operator /= (const Vec2_t& o) {
			this->x /= o.x;
			this->y /= o.y;

			return (*this);
		}

		inline Vec2_t& operator *= (const Vec2_t& o) {
			this->x *= o.x;
			this->y *= o.y;
			return *this;
		}

		inline Vec2_t& operator -= (const Vec2_t& o) {
			this->x -= o.x;
			this->y -= o.y;
			return *this;
		}

		inline Vec2_t& operator += (const Vec2_t& o) {
			this->x += o.x;
			this->y += o.y;
			return *this;
		}

		inline Vec2_t operator / (const Vec2_t& o) const {
			Vec2_t n = *this;
			n /= o;
			return n;
		}

		inline Vec2_t operator * (const Vec2_t& o) const {
			Vec2_t n = *this;
			n *= o;
			return n;
		}

		inline Vec2_t operator - (const Vec2_t& o) const {
			Vec2_t n = *this;
			n -= o;
			return n;
		}

		inline Vec2_t operator + (const Vec2_t& o) const {
			Vec2_t n = *this;
			n += o;
			return n;
		}

		inline float Dot(const Vec2_t &v) const
		{
			return (this->X * v.X + this->Y * v.Y);
		}

		inline float LengthSquared() const
		{
			return (this->Dot(*this));
		}

		inline float Length() const
		{
			return (ART::Sqrt(this->LengthSquared()));
		}

		inline bool IsValid() const
		{
			return ART::IsFloatFinite(X) && ART::IsFloatFinite(Y);
		}

		inline float Normalize() 
		{
			if(!this->IsValid())
			{
				return 0.f;
			}

			float l = this->Length();
			if(l != 0.f)
			{
				this->X /= l;
				this->Y /= l;
			}
			else
			{
				this->X = this->Y = 0.0f;
			}

			return l;
		}

		Vec2_t(const Vec2_t& other) : x(other.x), y(other.y) { };
		Vec2_t(float x, float y) : x(x), y(y) { };
		constexpr Vec2_t() : x(0), y(0) { };

		union {
			struct {
				float X, Y;
			};
			struct {
				float x, y;
			};
			float m[2];
		};
	};

	struct Vec3_t
	{
		Vec2_t ToVector2D() const
		{
			return Vec2_t(this->X, this->Y);
		}

		float Dot(const Vec3_t &v) const
		{
			return (this->X * v.X + this->Y * v.Y + this->Z * v.Z);
		}
		float Dot2(const Vec3_t &v) const
		{
			return this->X * v.Y + this->Y * v.X + this->Z * v.Z;
		}

		Vec3_t Cross(const Vec3_t &v) const
		{
			return Vec3_t(this->Y * v.Z - this->Z * v.Y, this->Z * v.X - this->X * v.Z, this->X * v.Y - this->Y * v.X);
		}

		float Dot(const float *v) const
		{
			return this->X * v[0] + this->Y * v[1] + this->Z * v[2];
		}

		float NormalizeInPlace()
		{
			const float Length = this->Length();
			const float Radius = 1.0f / (Length + EPSILON);

			this->X *= Radius;
			this->Y *= Radius;
			this->Z *= Radius;

			return Length;
		}
		
		Vec3_t Normalized() const
		{
			Vec3_t Out = *this;
			Out.NormalizeInPlace();
			return Out;
		}

		void ClampAngles()
		{
			this->X = ART::Clamp(this->X, -89.f, 89.f);
			this->Y = ART::Clamp(this->Y, -180.f, 180.f);
			this->Z = ART::Clamp(this->Z, -45.f, 45.f);
		}

		void NormalizeAngles()
		{
			if(this->X > 180.f || this->X < -180.f)
			{
				auto val = 360.f * ART::Round(ART::AbsF(this->X / 360.f));
				this->X < 0.f ? this->X += val : this->X -= val;
			}

			if(this->Y > 180.f || this->Y < -180.f)
			{
				auto val = 360.f * ART::Round(ART::AbsF(this->Y / 360.f));
				this->Y < 0.f ? this->Y += val : this->Y -= val;
			}

			if(this->Z > 180.f || this->Z < -180.f)
			{
				auto val = 360.f * ART::Round(ART::AbsF(this->Z / 360.f));
				this->Z < 0.f ? this->Z += val : this->Z -= val;
			}
		}

		void AngleToAngles(Vec3_t *Forward, Vec3_t *Right, Vec3_t *Up) const
		{
			float sp = ART::Sin(ART::DegreeToRadians(this->X));
			float cp = ART::Cos(ART::DegreeToRadians(this->X));
			float sy = ART::Sin(ART::DegreeToRadians(this->Y));
			float cy = ART::Cos(ART::DegreeToRadians(this->Y));
			float sr = ART::Sin(ART::DegreeToRadians(this->Z));
			float cr = ART::Cos(ART::DegreeToRadians(this->Z));

			if(Forward)
			{
				Forward->X = cp * cy;
				Forward->Y = cp * sy;
				Forward->Z = -sp;
			}

			if(Right)
			{
				Right->X = -1 * sr * sp * cy + -1 * cr * -sy;
				Right->Y = -1 * sr * sp * sy + -1 * cr * cy;
				Right->Z = -1 * sr * cp;
			}

			if(Up)
			{
				Up->X = cr * sp * cy + -sr * -sy;
				Up->Y = cr * sp * sy + -sr * cy;
				Up->Z = cr * cp;
			}
		}

		
		void DirectionToAngles(Vec3_t *Right, Vec3_t *Up) const
		{
			if(ART::AbsF(this->X) < 1e-6 && ART::AbsF(this->Y) < 1e-6)
			{
				if(Right)
				{
					Right->X = 0.f;
					Right->Y = -1.f;
					Right->Z = 0.f;
				}

				if(Up)
				{
					Up->X = -this->Z;
					Up->Y = 0.f;
					Up->Z = 0.f;
				}

			}
			else
			{
				Vec3_t TMP;
				TMP.X = 0.f; TMP.Y = 0.f; TMP.Z = 1.f;
				auto Right2 = Vec3_t(0.f, 0.f, 0.f);

				Right2 = this->Cross(TMP);
				if(Right2.LengthSquared() < 1e-6f)
				{
					Right2 = this->Cross(Vec3_t{1.f, 0.f, 0.f});
				}
				Right2 = Right2.Normalized();
				if(Up)
				{
					*Up = Right2.Cross(*this);
					*Up = Up->Normalized();
				}

				if(Right)
				{
					*Right = Right2;
				}
			}
		}

		Vec3_t DirectionToAngle() const
		{
			float Pitch, Yaw, Roll;

			const float Length = this->Length2D();

			if(Length > 0.f)
			{
				Pitch = ART::RadiansToDegree(ART::Atan2(-this->Z, Length));
				Yaw = ART::RadiansToDegree(ART::Atan2(this->Y, this->X));
				Roll = 0.f;
			}

			else
			{
				Pitch = float((this->Z > 0) ? 270 : 90);
				Yaw = 0.f;
				Roll = 0.f;
			}

			return Vec3_t{Pitch, Yaw, Roll};
		}
		
		bool IsEqual(const Vec3_t &Equal, const float ErrorMargin = EPSILON) const
		{
			return (ART::AbsF(this->X - Equal.X) < ErrorMargin &&
				ART::AbsF(this->Y - Equal.Y) < ErrorMargin &&
				ART::AbsF(this->Z - Equal.Z) < ErrorMargin);
		}

		float Length2DSqr() const
		{
			return (this->X * this->X + this->Y * this->Y);
		}

		float Length2D() const
		{
			return ART::Sqrt(this->Length2DSqr());
		}

		float LengthSquared() const
		{
			return (this->Dot(*this));
		}

		float Length() const
		{
			return (ART::Sqrt(this->LengthSquared()));
		}
		
		bool operator==(const Vec3_t Other) const
		{
			return this->IsEqual(Other);
		}
		
		bool operator!=(const Vec3_t Other) const
		{
			return !this->IsEqual(Other);
		}
		
		Vec3_t operator+(const Vec3_t &Other) const
		{
			return Vec3_t(this->X + Other.X, this->Y + Other.Y, this->Z + Other.Z);
		}

		Vec3_t operator-(const Vec3_t &Subtract) const
		{
			return Vec3_t(this->X - Subtract.X, this->Y - Subtract.Y, this->Z - Subtract.Z);
		}

		Vec3_t operator*(const Vec3_t &Multiply) const
		{
			return Vec3_t(this->X * Multiply.X, this->Y * Multiply.Y, this->Z * Multiply.Z);
		}
		
		Vec3_t operator/(const Vec3_t &Divide) const
		{
			return Vec3_t(this->X / Divide.X, this->Y / Divide.Y, this->Z / Divide.Z);
		}

		Vec3_t operator+(const float Add) const
		{
			return Vec3_t(this->X + Add, this->Y + Add, this->Z + Add);
		}

		Vec3_t operator-(const float Subtract) const
		{
			return Vec3_t(this->X - Subtract, this->Y - Subtract, this->Z - Subtract);
		}
		
		Vec3_t operator*(const float Multiply) const
		{
			return Vec3_t(this->X * Multiply, this->Y * Multiply, this->Z * Multiply);
		}

		Vec3_t operator/(const float Divide) const
		{
			return Vec3_t(this->X / Divide, this->Y / Divide, this->Z / Divide);
		}
		
		constexpr Vec3_t &operator+=(const Vec3_t &Other)
		{
			this->X += Other.X; this->Y += Other.Y; this->Z += Other.Z;
			return *this;
		}

		constexpr Vec3_t &operator-=(const Vec3_t &Other)
		{
			this->X -= Other.X; this->Y -= Other.Y; this->Z -= Other.Z;
			return *this;
		}

		constexpr Vec3_t &operator*=(const Vec3_t &Other)
		{
			this->X *= Other.X; this->Y *= Other.Y; this->Z *= Other.Z;
			return *this;
		}

		constexpr Vec3_t &operator/=(const Vec3_t &Other)
		{
			this->X /= Other.X; this->Y /= Other.Y; this->Z /= Other.Z;
			return *this;
		}
		
		constexpr Vec3_t &operator+=(const float Add)
		{
			this->X += Add; this->Y += Add; this->Z += Add;
			return *this;
		}

		constexpr Vec3_t &operator-=(const float Subtract)
		{
			this->X -= Subtract; this->Y -= Subtract; this->Z -= Subtract;
			return *this;
		}

		constexpr Vec3_t &operator*=(const float Multiply)
		{
			this->X *= Multiply; this->Y *= Multiply; this->Z *= Multiply;
			return *this;
		}

		constexpr Vec3_t &operator/=(const float Divide)
		{
			this->X /= Divide; this->Y /= Divide; this->Z /= Divide;
			return *this;
		}

		float *ToArray() const
		{
			static float Arr[3] = {X, Y, Z};
			return Arr;
		}

		Vec3_t(const Vec3_t& other) : X(other.x), Y(other.y), Z(other.z) { };

		Vec3_t(int x, int y, int z) : X((float)(x)), Y((float)(y)), Z((float)(z)) {};

		Vec3_t(float x, float y, float z) : X(x), Y(y), Z(z) {};

		Vec3_t() : X(0.f), Y(0.f), Z(0.f) {};

		constexpr Vec3_t(const float* Arr) : X(Arr[0]), Y(Arr[1]), Z(Arr[2]) {}

		union {
			struct {
				float X, Y, Z;
			};
			struct {
				float x; 
				union {
					struct {
						float y, z;
					};
					Vec2_t yz;
				};
			};
			float m[3];
			Vec2_t xy;
		};
	};

	struct Vec4_t
	{
		Vec4_t(const Vec3_t &other, const float OtherW) : X(other.x), Y(other.y), Z(other.z), W(OtherW) { };
		Vec4_t(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {};
		Vec4_t() : X(0.f), Y(0.f), Z(0.f), W(0.f) {};

		union {
			struct {
				float X, Y, Z, W;
			};
			struct {
				float x, y, z, w;
			};
			float m[4];
		};
	};
}