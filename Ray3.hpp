#ifndef Ray3_H
#define Ray3_H

#include "OrientedBox.hpp"

class Ray3
{
public:
	Vector3 Position, Direction;

	Ray3();
	~Ray3();
	Ray3(const Ray3&);
	Ray3(Ray3&&) noexcept;
	Ray3(const Vector3&, const Vector3&);
	Ray3& operator=(const Ray3&);
	Ray3& operator=(Ray3&&) noexcept;

	bool Picks(const OrientedBox&, float&) const;
};

#endif