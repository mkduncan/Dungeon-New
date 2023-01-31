#ifndef Vector4_H
#define Vector4_H

#include <array>

class Vector4
{
private:
	std::array<float, 4> Row;

public:
	Vector4();
	~Vector4();
	Vector4(const Vector4&);
	Vector4(Vector4&&) noexcept;
	Vector4(const float);
	Vector4(const float, const float, const float, const float);

	Vector4& operator=(const Vector4&);
	Vector4& operator=(Vector4&&) noexcept;
	const float& operator[](const size_t) const;
	float& operator[](const size_t);
};

#endif