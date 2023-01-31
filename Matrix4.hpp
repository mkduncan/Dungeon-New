#ifndef Matrix4_H
#define Matrix4_H

#include "Vector3.hpp"
#include "Vector4.hpp"

class Matrix4
{
private:
	std::array<Vector4, 4> Rows;

public:
	Matrix4();
	~Matrix4();
	Matrix4(const Matrix4&);
	Matrix4(Matrix4&&) noexcept;
	Matrix4(const Vector4&, const Vector4&, const Vector4&, const Vector4&);
	Matrix4(const float, const float, const float, const float, const float, const float, const float, const float, const float, const float, const float, const float, const float, const float, const float, const float);

	Matrix4& operator=(const Matrix4&);
	Matrix4& operator=(Matrix4&&) noexcept;
	const Vector4& operator[](const size_t) const;
	Vector4& operator[](const size_t);

	const Matrix4 operator*(const Matrix4&) const;
	const Vector3 operator*(const Vector3&) const;

	static const Matrix4 Perspective(const float, const float, const float, const float);
	static const Matrix4 LookAt(const Vector3&, const Vector3&, const Vector3&);
	static const Matrix4 Scaled(const Vector3&);
	static const Matrix4 Translated(const Vector3&);
	static const Matrix4 Rotated(const Vector3&);
	static const Matrix4 Inverse(const Matrix4&);
};

#endif