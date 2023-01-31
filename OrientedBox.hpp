#ifndef OrientedBox_H
#define OrientedBox_H

#include "AlignedBox.hpp"
#include "Matrix4.hpp"

class OrientedBox
{
public:
	Matrix4 Transform;
	Vector3 Minimum, Maximum;

	OrientedBox();
	~OrientedBox();
	OrientedBox(const OrientedBox&);
	OrientedBox(OrientedBox&&) noexcept;
	OrientedBox& operator=(const OrientedBox&);
	OrientedBox& operator=(OrientedBox&&) noexcept;

	const Matrix4 GetTransform() const;
	bool SetTransform(const Matrix4&);
	bool SetBounds(const AlignedBox&, const Matrix4& = Matrix4());

	const bool Render() const;
};

#endif