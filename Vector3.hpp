#ifndef Vector3_H
#define Vector3_H

#include <array>

class Vector3
{
private:
	std::array<float, 3> Row;

public:
	static Vector3 Right, Left, Up, Down, Out, In;

	Vector3();
	~Vector3();
	Vector3(const Vector3&);
	Vector3(Vector3&&) noexcept;
	Vector3(const float);
	Vector3(const float, const float, const float);

	Vector3& operator=(const Vector3&);
	Vector3& operator=(Vector3&&) noexcept;
	const float& operator[](const size_t) const;
	float& operator[](const size_t);

	Vector3& operator+=(const Vector3&);
	Vector3& operator-=(const Vector3&);
	const Vector3 operator+(const Vector3&) const;
	const Vector3 operator-(const Vector3&) const;
	const Vector3 operator*(const float) const;

	static const Vector3 Cross(const Vector3&, const Vector3&);
	static const float Dot(const Vector3&, const Vector3&);
	static const Vector3 Normalize(const Vector3&);
};

#endif