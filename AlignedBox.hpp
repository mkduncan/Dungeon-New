#ifndef AlignedBox_H
#define AlignedBox_H

#include "Vector3.hpp"

class AlignedBox
{
public:
	Vector3 Minimum, Maximum;
	size_t Id;

	friend class DungeonManager;
	friend class DungeonMap;
	friend class Model;

public:
	AlignedBox();
	~AlignedBox();
	AlignedBox(const AlignedBox&);
	AlignedBox(AlignedBox&&) noexcept;
	AlignedBox& operator=(const AlignedBox&);
	AlignedBox& operator=(AlignedBox&&) noexcept;

	bool Envelope(const Vector3&);
	bool Overlaps(const AlignedBox&) const;

	const std::array<Vector3, 8> Corners() const;
	const Vector3 Center() const;
	const Vector3 Extent() const;

	const bool Render() const;
};

#endif