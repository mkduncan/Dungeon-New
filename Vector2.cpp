#include "Vector2.hpp"

Vector2::Vector2() : Row({ 0.0f, 0.0f })
{
}

Vector2::~Vector2()
{
}

Vector2::Vector2(const Vector2& row) : Row(row.Row)
{
}

Vector2::Vector2(Vector2&& row) noexcept : Row(std::move(row.Row))
{
}

Vector2::Vector2(const float scalar) : Row({ scalar, scalar })
{
}

Vector2::Vector2(const float scalarX, const float scalarY) : Row({ scalarX, scalarY })
{
}

Vector2& Vector2::operator=(const Vector2& row)
{
	this->Row = row.Row;
	return (*this);
}

Vector2& Vector2::operator=(Vector2&& row) noexcept
{
	this->Row = std::move(row.Row);
	return (*this);
}

const float& Vector2::operator[](const size_t index) const
{
	return this->Row[index];
}

float& Vector2::operator[](const size_t index)
{
	return this->Row[index];
}

const Vector2 Vector2::operator*(const float scalar) const
{
	return Vector2(this->Row[0] * scalar, this->Row[1] * scalar);
}