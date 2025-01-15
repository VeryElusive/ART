#pragma once

#include "../common/decl.hpp"	

#include "vector.hpp"

#define __bit_cast(val) (*(__m128*)&(val))

namespace ART
{
	struct Matrix3x4_t
	{
		Matrix3x4_t() = default;

		Matrix3x4_t(
			const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23)
		{
			Data[0][0] = m00;
			Data[0][1] = m01;
			Data[0][2] = m02;
			Data[0][3] = m03;
			Data[1][0] = m10;
			Data[1][1] = m11;
			Data[1][2] = m12;
			Data[1][3] = m13;
			Data[2][0] = m20;
			Data[2][1] = m21;
			Data[2][2] = m22;
			Data[2][3] = m23;
		}
		
		Matrix3x4_t(const Vec3_t &Forward, const Vec3_t &Left, const Vec3_t &Up, const Vec3_t &Origin)
		{
			SetForward(Forward);
			SetLeft(Left);
			SetUp(Up);
			SetOrigin(Origin);
		}

		float *operator[](const int Index)
		{
			return Data[Index];
		}
		
		const float *operator[](const int Index) const
		{
			return Data[Index];
		}

		Matrix3x4_t operator+(const Matrix3x4_t &Other) const 
		{
			Matrix3x4_t ret;
			auto &m = Data;
			for(int i = 0; i < 12; i++) 
			{
				((float *)ret.Data)[i] = ((float *)m)[i] + ((float *)Other.Data)[i];
			}
			return ret;
		}

		Matrix3x4_t operator*(const Matrix3x4_t &vm) const 
		{
			return ConcatTransforms(vm);
		}
		
		Matrix3x4_t operator*(const float &Other) const 
		{
			Matrix3x4_t ret;
			auto &m = Data;
			for(int i = 0; i < 12; i++) 
			{
				((float *)ret.Data)[i] = ((float *)m)[i] * Other;
			}
			return ret;
		}

		Vec3_t operator*(const Vec3_t &Other) const 
		{
			auto &m = Data;
			Vec3_t vRet;
			vRet.X = m[0][0] * Other.X + m[0][1] * Other.Y + m[0][2] * Other.Z + m[0][3];
			vRet.Y = m[1][0] * Other.X + m[1][1] * Other.Y + m[1][2] * Other.Z + m[1][3];
			vRet.Z = m[2][0] * Other.X + m[2][1] * Other.Y + m[2][2] * Other.Z + m[2][3];

			return vRet;
		}
		
		Vec3_t Transform(const Vec3_t &vector) const 
		{
			return 
			{
				vector.Dot(Data[0]) + Data[0][3],
				vector.Dot(Data[1]) + Data[1][3],
				vector.Dot(Data[2]) + Data[2][3]
			};
		}

		void SetForward(const Vec3_t &Forward)
		{
			Data[0][0] = Forward.X;
			Data[1][0] = Forward.Y;
			Data[2][0] = Forward.Z;
		}

		void SetLeft(const Vec3_t &Left)
		{
			Data[0][1] = Left.X;
			Data[1][1] = Left.Y;
			Data[2][1] = Left.Z;
		}
		
		void SetUp(const Vec3_t &Up)
		{
			Data[0][2] = Up.X;
			Data[1][2] = Up.Y;
			Data[2][2] = Up.Z;
		}

		void SetOrigin(const Vec3_t &Origin)
		{
			Data[0][3] = Origin.X;
			Data[1][3] = Origin.Y;
			Data[2][3] = Origin.Z;
		}
		
		Vec3_t GetForward() const
		{
			return {Data[0][0], Data[1][0], Data[2][0]};
		}

		Vec3_t GetLeft() const
		{
			return {Data[0][1], Data[1][1], Data[2][1]};
		}
		
		Vec3_t GetUp() const
		{
			return {Data[0][2], Data[1][2], Data[2][2]};
		}

		Vec3_t GetOrigin() const
		{
			return {Data[0][3], Data[1][3], Data[2][3]};
		}

		void Invalidate()
		{
			for(auto &SubData : Data)
			{
				for(auto &Data : SubData)
				{
					Data = __builtin_huge_valf();
				}
			}
		}

		Matrix3x4_t ConcatTransforms(const Matrix3x4_t &Other) const
		{
			return {
				Data[0][0] * Other.Data[0][0] + Data[0][1] * Other.Data[1][0] + Data[0][2] * Other.Data[2][0],
				Data[0][0] * Other.Data[0][1] + Data[0][1] * Other.Data[1][1] + Data[0][2] * Other.Data[2][1],
				Data[0][0] * Other.Data[0][2] + Data[0][1] * Other.Data[1][2] + Data[0][2] * Other.Data[2][2],
				Data[0][0] * Other.Data[0][3] + Data[0][1] * Other.Data[1][3] + Data[0][2] * Other.Data[2][3] + Data[0][3],

				Data[1][0] * Other.Data[0][0] + Data[1][1] * Other.Data[1][0] + Data[1][2] * Other.Data[2][0],
				Data[1][0] * Other.Data[0][1] + Data[1][1] * Other.Data[1][1] + Data[1][2] * Other.Data[2][1],
				Data[1][0] * Other.Data[0][2] + Data[1][1] * Other.Data[1][2] + Data[1][2] * Other.Data[2][2],
				Data[1][0] * Other.Data[0][3] + Data[1][1] * Other.Data[1][3] + Data[1][2] * Other.Data[2][3] + Data[1][3],

				Data[2][0] * Other.Data[0][0] + Data[2][1] * Other.Data[1][0] + Data[2][2] * Other.Data[2][0],
				Data[2][0] * Other.Data[0][1] + Data[2][1] * Other.Data[1][1] + Data[2][2] * Other.Data[2][1],
				Data[2][0] * Other.Data[0][2] + Data[2][1] * Other.Data[1][2] + Data[2][2] * Other.Data[2][2],
				Data[2][0] * Other.Data[0][3] + Data[2][1] * Other.Data[1][3] + Data[2][2] * Other.Data[2][3] + Data[2][3]
			};
		}

		float Data[3][4] = {};
	};
}

