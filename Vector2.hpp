#ifndef Vector2_H
#define Vector2_H

#include <array>

class Vector2
{
private:
	std::array<float, 2> Row;

public:
	Vector2();
	~Vector2();
	Vector2(const Vector2&);
	Vector2(Vector2&&) noexcept;
	Vector2(const float);
	Vector2(const float, const float);

	Vector2& operator=(const Vector2&);
	Vector2& operator=(Vector2&&) noexcept;
	const float& operator[](const size_t) const;
	float& operator[](const size_t);

	const Vector2 operator*(const float) const;
};

#endif