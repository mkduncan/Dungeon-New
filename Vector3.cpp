#include "Vector3.hpp"
#include <cmath>

Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
Vector3 Vector3::Out(0.0f, 0.0f, 1.0f);
Vector3 Vector3::In(0.0f, 0.0f, -1.0f);

Vector3::Vector3() : Row({ 0.0f, 0.0f, 0.0f })
{
}

Vector3::~Vector3()
{
}

Vector3::Vector3(const Vector3& row) : Row(row.Row)
{
}

Vector3::Vector3(Vector3&& row) noexcept : Row(std::move(row.Row))
{
}

Vector3::Vector3(const float scalar) : Row({ scalar, scalar, scalar })
{
}

Vector3::Vector3(const float scalarX, const float scalarY, const float scalarZ) : Row({ scalarX, scalarY, scalarZ })
{
}

Vector3& Vector3::operator=(const Vector3& row)
{
	this->Row = row.Row;
	return (*this);
}

Vector3& Vector3::operator=(Vector3&& row) noexcept
{
	this->Row = std::move(row.Row);
	return (*this);
}

const float& Vector3::operator[](const size_t index) const
{
	return this->Row[index];
}

float& Vector3::operator[](const size_t index)
{
	return this->Row[index];
}

Vector3& Vector3::operator+=(const Vector3& value)
{
	this->Row[0] += value[0];
	this->Row[1] += value[1];
	this->Row[2] += value[2];

	return (*this);
}

Vector3& Vector3::operator-=(const Vector3& value)
{
	this->Row[0] -= value[0];
	this->Row[1] -= value[1];
	this->Row[2] -= value[2];

	return (*this);
}

const Vector3 Vector3::operator+(const Vector3& value) const
{
	return Vector3(this->Row[0] + value[0], this->Row[1] + value[1], this->Row[2] + value[2]);
}

const Vector3 Vector3::operator-(const Vector3& value) const
{
	return Vector3(this->Row[0] - value[0], this->Row[1] - value[1], this->Row[2] - value[2]);
}

const Vector3 Vector3::operator*(const float value) const
{
	return Vector3(this->Row[0] * value, this->Row[1] * value, this->Row[2] * value);
}

const Vector3 Vector3::Cross(const Vector3& valueLeft, const Vector3& valueRight)
{
	return Vector3(valueLeft[1] * valueRight[2] - valueLeft[2] * valueRight[1], valueLeft[2] * valueRight[0] - valueLeft[0] * valueRight[2], valueLeft[0] * valueRight[1] - valueLeft[1] * valueRight[0]);
}

const float Vector3::Dot(const Vector3& valueLeft, const Vector3& valueRight)
{
	return valueLeft[0] * valueRight[0] + valueLeft[1] * valueRight[1] + valueLeft[2] * valueRight[2];
}

const Vector3 Vector3::Normalize(const Vector3& value)
{
	return value * (1.0f / sqrtf(Vector3::Dot(value, value)));
}