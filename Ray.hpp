#ifndef Ray_H
#define Ray_H

#include "ModelMesh.hpp"

class Ray
{
public:
	static glm::vec3 Position, Direction;

	Ray() = delete;
	~Ray() = delete;
	Ray(const Ray&) = delete;
	Ray(Ray&&) noexcept = delete;
	Ray& operator=(const Ray&) = delete;
	Ray& operator=(Ray&&) = delete;

	static bool Picked(const ModelMesh&, float&);
	static bool Update();
};

#endif