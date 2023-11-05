#pragma once

namespace MathUtil
{
	template <typename T>
	class Vector3
	{
	public:
		Vector3() {}
		Vector3(const Vector3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}
		Vector3 operator+(const Vector3& rhs) const
		{
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}
		Vector3 operator-(const Vector3& rhs) const
		{
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}

	public:
		T x;
		T y;
		T z;
	};

	template <typename T>
	class Vector4
	{
	public:
		Vector4() {}
		Vector4(const class Vector3<T>& _v, T _w)
		{
			v = _v;
			w = _w;
		}
		Vector4(const Vector4<T>& other)
		{
			v = other.v;
			w = other.w;
		}
		Vector4 operator+(const Vector4<T>& rhs) const
		{
			return Vector4<T>(v + rhs.v, w + rhs.w);
		}
		Vector4 operator-(const Vector4<T>& rhs) const
		{
			return Vector4<T>(v - rhs.v, w - rhs.w);
		}

	public:
		class Vector3<T> v;
		T w;
	};
}


