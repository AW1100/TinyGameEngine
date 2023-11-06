#pragma once
#include "Misc.h"

namespace MathUtil
{
	template <typename T>
	class Matrix4;

	template <typename T>
	class Matrix3
	{
	public:
		Matrix3(T _00 = 0.0f, T _01 = 0.0f, T _02 = 0.0f,
			T _10 = 0.0f, T _11 = 0.0f, T _12 = 0.0f,
			T _20 = 0.0f, T _21 = 0.0f, T _22 = 0.0f)
		{
			m[0][0] = _00; m[0][1] = _01; m[0][2] = _02;
			m[1][0] = _10; m[1][1] = _11; m[1][2] = _12;
			m[2][0] = _20; m[2][1] = _21; m[2][2] = _22;
		}

		Matrix3(const Matrix4<T>& other)
		{
			m[0][0] = other.m[0][0]; m[0][1] = other.m[0][1]; m[0][2] = other.m[0][2];
			m[1][0] = other.m[1][0]; m[1][1] = other.m[1][1]; m[1][2] = other.m[1][2];
			m[2][0] = other.m[2][0]; m[2][1] = other.m[2][1]; m[2][2] = other.m[2][2];
		}

		friend std::ostream& operator<<(std::ostream& os, const Matrix3<T>& obj)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					os << obj.m[i][j];
					os << " ";
				}
				os << std::endl;
			}
			
			return os;
		}

	public:
		T m[3][3];
	};

	template <typename T>
	class Matrix4
	{
	public:
		Matrix4(T _00 = 0.0f, T _01 = 0.0f, T _02 = 0.0f, T _03 = 0.0f,
			T _10 = 0.0f, T _11 = 0.0f, T _12 = 0.0f, T _13 = 0.0f,
			T _20 = 0.0f, T _21 = 0.0f, T _22 = 0.0f, T _23 = 0.0f,
			T _30 = 0.0f, T _31 = 0.0f, T _32 = 0.0f, T _33 = 1.0f)
		{
			m[0][0] = _00; m[0][1] = _01; m[0][2] = _02; m[0][3] = _03;
			m[1][0] = _10; m[1][1] = _11; m[1][2] = _12; m[1][3] = _13;
			m[2][0] = _20; m[2][1] = _21; m[2][2] = _22; m[2][3] = _23;
			m[3][0] = _30; m[3][1] = _31; m[3][2] = _32; m[3][3] = _33;
		}
		Matrix4(const Matrix3<T>& other)
		{
			m[0][0] = other.m[0][0]; m[0][1] = other.m[0][1]; m[0][2] = other.m[0][2]; m[0][3] = 0.0f;
			m[1][0] = other.m[1][0]; m[1][1] = other.m[1][1]; m[1][2] = other.m[1][2]; m[1][3] = 0.0f;
			m[2][0] = other.m[2][0]; m[2][1] = other.m[2][1]; m[2][2] = other.m[2][2]; m[2][3] = 0.0f;
			m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
		}

		friend std::ostream& operator<<(std::ostream& os, const Matrix4<T>& obj)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					os << obj.m[i][j];
					os << " ";
				}
				os << std::endl;
			}

			return os;
		}

	public:
		T m[4][4];
	};
}



