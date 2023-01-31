#include "Vector4.hpp"

Vector4::Vector4() : Row({ 0.0f, 0.0f, 0.0f, 0.0f })
{
}

Vector4::~Vector4()
{
}

Vector4::Vector4(const Vector4& row) : Row(row.Row)
{
}

Vector4::Vector4(Vector4&& row) noexcept : Row(std::move(row.Row))
{
}

Vector4::Vector4(const float scalar) : Row({ scalar, scalar, scalar, scalar })
{
}

Vector4::Vector4(const float scalarX, const float scalarY, const float scalarZ, const float scalarW) : Row({ scalarX, scalarY, scalarZ, scalarW })
{
}

Vector4& Vector4::operator=(const Vector4& row)
{
	this->Row = row.Row;
	return (*this);
}

Vector4& Vector4::operator=(Vector4&& row) noexcept
{
	this->Row = std::move(row.Row);
	return (*this);
}

const float& Vector4::operator[](const size_t index) const
{
	return this->Row[index];
}

float& Vector4::operator[](const size_t index)
{
	return this->Row[index];
}