#include "Matrix4.hpp"
#include <cmath>

Matrix4::Matrix4() : Rows({ Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f) })
{
}

Matrix4::~Matrix4()
{
}

Matrix4::Matrix4(const Matrix4& Rowss) : Rows(Rowss.Rows)
{
}

Matrix4::Matrix4(Matrix4&& Rowss) noexcept : Rows(std::move(Rowss.Rows))
{
}

Matrix4::Matrix4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3) : Rows({ r0, r1, r2, r3 })
{
}

Matrix4::Matrix4(const float r00, const float r01, const float r02, const float r03, const float r10, const float r11, const float r12, const float r13, const float r20, const float r21, const float r22, const float r23, const float r30, const float r31, const float r32, const float r33) : Rows({ Vector4(r00, r01, r02, r03), Vector4(r10, r11, r12, r13), Vector4(r20, r21, r22, r23), Vector4(r30, r31, r32, r33) })
{
}

Matrix4& Matrix4::operator=(const Matrix4& Rowss)
{
	this->Rows = Rowss.Rows;
	return (*this);
}

Matrix4& Matrix4::operator=(Matrix4&& Rowss) noexcept
{
	this->Rows = std::move(Rowss.Rows);
	return (*this);
}

const Vector4& Matrix4::operator[](const size_t index) const
{
	return this->Rows[index];
}

Vector4& Matrix4::operator[](const size_t index)
{
	return this->Rows[index];
}

const Matrix4 Matrix4::operator*(const Matrix4& matrix) const
{
	return Matrix4
	(
		this->Rows[0][0] * matrix.Rows[0][0] + this->Rows[1][0] * matrix.Rows[0][1] + this->Rows[2][0] * matrix.Rows[0][2] + this->Rows[3][0] * matrix.Rows[0][3],
		this->Rows[0][1] * matrix.Rows[0][0] + this->Rows[1][1] * matrix.Rows[0][1] + this->Rows[2][1] * matrix.Rows[0][2] + this->Rows[3][1] * matrix.Rows[0][3],
		this->Rows[0][2] * matrix.Rows[0][0] + this->Rows[1][2] * matrix.Rows[0][1] + this->Rows[2][2] * matrix.Rows[0][2] + this->Rows[3][2] * matrix.Rows[0][3],
		this->Rows[0][3] * matrix.Rows[0][0] + this->Rows[1][3] * matrix.Rows[0][1] + this->Rows[2][3] * matrix.Rows[0][2] + this->Rows[3][3] * matrix.Rows[0][3],
		this->Rows[0][0] * matrix.Rows[1][0] + this->Rows[1][0] * matrix.Rows[1][1] + this->Rows[2][0] * matrix.Rows[1][2] + this->Rows[3][0] * matrix.Rows[1][3],
		this->Rows[0][1] * matrix.Rows[1][0] + this->Rows[1][1] * matrix.Rows[1][1] + this->Rows[2][1] * matrix.Rows[1][2] + this->Rows[3][1] * matrix.Rows[1][3],
		this->Rows[0][2] * matrix.Rows[1][0] + this->Rows[1][2] * matrix.Rows[1][1] + this->Rows[2][2] * matrix.Rows[1][2] + this->Rows[3][2] * matrix.Rows[1][3],
		this->Rows[0][3] * matrix.Rows[1][0] + this->Rows[1][3] * matrix.Rows[1][1] + this->Rows[2][3] * matrix.Rows[1][2] + this->Rows[3][3] * matrix.Rows[1][3],
		this->Rows[0][0] * matrix.Rows[2][0] + this->Rows[1][0] * matrix.Rows[2][1] + this->Rows[2][0] * matrix.Rows[2][2] + this->Rows[3][0] * matrix.Rows[2][3],
		this->Rows[0][1] * matrix.Rows[2][0] + this->Rows[1][1] * matrix.Rows[2][1] + this->Rows[2][1] * matrix.Rows[2][2] + this->Rows[3][1] * matrix.Rows[2][3],
		this->Rows[0][2] * matrix.Rows[2][0] + this->Rows[1][2] * matrix.Rows[2][1] + this->Rows[2][2] * matrix.Rows[2][2] + this->Rows[3][2] * matrix.Rows[2][3],
		this->Rows[0][3] * matrix.Rows[2][0] + this->Rows[1][3] * matrix.Rows[2][1] + this->Rows[2][3] * matrix.Rows[2][2] + this->Rows[3][3] * matrix.Rows[2][3],
		this->Rows[0][0] * matrix.Rows[3][0] + this->Rows[1][0] * matrix.Rows[3][1] + this->Rows[2][0] * matrix.Rows[3][2] + this->Rows[3][0] * matrix.Rows[3][3],
		this->Rows[0][1] * matrix.Rows[3][0] + this->Rows[1][1] * matrix.Rows[3][1] + this->Rows[2][1] * matrix.Rows[3][2] + this->Rows[3][1] * matrix.Rows[3][3],
		this->Rows[0][2] * matrix.Rows[3][0] + this->Rows[1][2] * matrix.Rows[3][1] + this->Rows[2][2] * matrix.Rows[3][2] + this->Rows[3][2] * matrix.Rows[3][3],
		this->Rows[0][3] * matrix.Rows[3][0] + this->Rows[1][3] * matrix.Rows[3][1] + this->Rows[2][3] * matrix.Rows[3][2] + this->Rows[3][3] * matrix.Rows[3][3]
	);
}

const Vector3 Matrix4::operator*(const Vector3& vector) const
{
	return Vector3
	(
		vector[0] * this->Rows[0][0] + vector[1] * this->Rows[0][1] + vector[2] * this->Rows[0][2] + this->Rows[3][0],
		vector[0] * this->Rows[1][0] + vector[1] * this->Rows[1][1] + vector[2] * this->Rows[1][2] + this->Rows[3][1],
		vector[0] * this->Rows[2][0] + vector[1] * this->Rows[2][1] + vector[2] * this->Rows[2][2] + this->Rows[3][2]
	);
}

const Matrix4 Matrix4::Perspective(const float fieldOfView, const float aspectRatio, const float nearClipZ, const float farClipZ)
{
	float alpha = tanf(fieldOfView * 0.5f);

	return Matrix4
	(
		Vector4(1.0f / (aspectRatio * alpha), 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f / alpha, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, -(farClipZ + nearClipZ) / (farClipZ - nearClipZ), -1.0f),
		Vector4(0.0f, 0.0f, -(2.0f * farClipZ * nearClipZ) / (farClipZ - nearClipZ), 1.0f)
	);
}

const Matrix4 Matrix4::LookAt(const Vector3& eyePoint, const Vector3& centerPoint, const Vector3& upDirection)
{
	Vector3 alpha = Vector3::Normalize(centerPoint - eyePoint), beta = Vector3::Normalize(Vector3::Cross(alpha, upDirection)), gamma = Vector3::Cross(beta, alpha);

	return Matrix4
	(
		Vector4(beta[0], gamma[0], -alpha[0], 0.0f),
		Vector4(beta[1], gamma[1], -alpha[1], 0.0f),
		Vector4(beta[2], gamma[2], -alpha[2], 0.0f),
		Vector4(-Vector3::Dot(beta, eyePoint), -Vector3::Dot(gamma, eyePoint), Vector3::Dot(alpha, eyePoint), 1.0f)
	);
}

const Matrix4 Matrix4::Scaled(const Vector3& scale)
{
	return Matrix4
	(
		Vector4(scale[0], 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, scale[1], 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, scale[2], 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

const Matrix4 Matrix4::Translated(const Vector3& translation)
{
	return Matrix4
	(
		Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(translation[0], translation[1], translation[2], 1.0f)
	);
}

const Matrix4 Matrix4::Rotated(const Vector3& rotation)
{
	const float gamma[2] = { std::cosf(rotation[0]), std::sinf(rotation[0]) }, alpha[2] = { std::cosf(rotation[1]), std::sinf(rotation[1]) }, beta[2] = { std::cosf(rotation[2]), std::sinf(rotation[2]) };

	return Matrix4
	(
		Vector4(alpha[0] * beta[0], beta[1], -beta[0] * alpha[1], 0.0f),
		Vector4(alpha[1] * gamma[1] - alpha[0] * gamma[0] * beta[1], beta[0] * gamma[0], alpha[0] * gamma[1] + gamma[0] * alpha[1] * beta[1], 0.0f),
		Vector4(gamma[0] * alpha[1] + alpha[0] * beta[1] * gamma[1], -beta[0] * gamma[1], alpha[0] * gamma[0] - alpha[1] * beta[1] * gamma[1], 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

const Matrix4 Matrix4::Inverse(const Matrix4& matrix)
{
	float A2323 = matrix[2][2] * matrix[3][3] - matrix[2][3] * matrix[3][2];
	float A1323 = matrix[2][1] * matrix[3][3] - matrix[2][3] * matrix[3][1];
	float A1223 = matrix[2][1] * matrix[3][2] - matrix[2][2] * matrix[3][1];
	float A0323 = matrix[2][0] * matrix[3][3] - matrix[2][3] * matrix[3][0];
	float A0223 = matrix[2][0] * matrix[3][2] - matrix[2][2] * matrix[3][0];
	float A0123 = matrix[2][0] * matrix[3][1] - matrix[2][1] * matrix[3][0];
	float A2313 = matrix[1][2] * matrix[3][3] - matrix[1][3] * matrix[3][2];
	float A1313 = matrix[1][1] * matrix[3][3] - matrix[1][3] * matrix[3][1];
	float A1213 = matrix[1][1] * matrix[3][2] - matrix[1][2] * matrix[3][1];
	float A2312 = matrix[1][2] * matrix[2][3] - matrix[1][3] * matrix[2][2];
	float A1312 = matrix[1][1] * matrix[2][3] - matrix[1][3] * matrix[2][1];
	float A1212 = matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1];
	float A0313 = matrix[1][0] * matrix[3][3] - matrix[1][3] * matrix[3][0];
	float A0213 = matrix[1][0] * matrix[3][2] - matrix[1][2] * matrix[3][0];
	float A0312 = matrix[1][0] * matrix[2][3] - matrix[1][3] * matrix[2][0];
	float A0212 = matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0];
	float A0113 = matrix[1][0] * matrix[3][1] - matrix[1][1] * matrix[3][0];
	float A0112 = matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0];
	float det = matrix[0][0] * (matrix[1][1] * A2323 - matrix[1][2] * A1323 + matrix[1][3] * A1223) - matrix[0][1] * (matrix[1][0] * A2323 - matrix[1][2] * A0323 + matrix[1][3] * A0223) + matrix[0][2] * (matrix[1][0] * A1323 - matrix[1][1] * A0323 + matrix[1][3] * A0123) - matrix[0][3] * (matrix[1][0] * A1223 - matrix[1][1] * A0223 + matrix[1][2] * A0123);

	det = 1.0f / det;

	return Matrix4
	(
		det * (matrix[1][1] * A2323 - matrix[1][2] * A1323 + matrix[1][3] * A1223),
		det * -(matrix[0][1] * A2323 - matrix[0][2] * A1323 + matrix[0][3] * A1223),
		det * (matrix[0][1] * A2313 - matrix[0][2] * A1313 + matrix[0][3] * A1213),
		det * -(matrix[0][1] * A2312 - matrix[0][2] * A1312 + matrix[0][3] * A1212),
		det * -(matrix[1][0] * A2323 - matrix[1][2] * A0323 + matrix[1][3] * A0223),
		det * (matrix[0][0] * A2323 - matrix[0][2] * A0323 + matrix[0][3] * A0223),
		det * -(matrix[0][0] * A2313 - matrix[0][2] * A0313 + matrix[0][3] * A0213),
		det * (matrix[0][0] * A2312 - matrix[0][2] * A0312 + matrix[0][3] * A0212),
		det * (matrix[1][0] * A1323 - matrix[1][1] * A0323 + matrix[1][3] * A0123),
		det * -(matrix[0][0] * A1323 - matrix[0][1] * A0323 + matrix[0][3] * A0123),
		det * (matrix[0][0] * A1313 - matrix[0][1] * A0313 + matrix[0][3] * A0113),
		det * -(matrix[0][0] * A1312 - matrix[0][1] * A0312 + matrix[0][3] * A0112),
		det * -(matrix[1][0] * A1223 - matrix[1][1] * A0223 + matrix[1][2] * A0123),
		det * (matrix[0][0] * A1223 - matrix[0][1] * A0223 + matrix[0][2] * A0123),
		det * -(matrix[0][0] * A1213 - matrix[0][1] * A0213 + matrix[0][2] * A0113),
		det * (matrix[0][0] * A1212 - matrix[0][1] * A0212 + matrix[0][2] * A0112)
	);
}