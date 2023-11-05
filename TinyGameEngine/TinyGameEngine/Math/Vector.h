#pragma once

class Vector3
{
public:
	Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) :
		x(_x), y(_y), z(_z)
	{}

	Vector3 operator+(const Vector3& rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector3 operator-(const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

public:
	float x;
	float y;
	float z;
};

class Vector4
{
public:
	Vector4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) :
		v(_x, _y, _z), w(1.0)
	{}

	Vector4(float _x, float _y, float _z, float _w) :
		v(_x, _y, _z), w(_w)
	{}

public:
	Vector3 v;
	float w;
};
