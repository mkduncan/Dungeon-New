#ifndef Frustum_H
#define Frustum_H

#include "ModelMesh.hpp"

//	Based on code from "https://gist.github.com/podgorskiy/e698d18879588ada9014768e3e82a644"
//	as written by user: "podgorskiy"; also known as: "Stanislav Pidhorskyi".

class Frustum
{
public:
	static glm::vec4 Plane[6];
	static glm::vec3 Point[8];

	Frustum() = delete;
	~Frustum() = delete;
	Frustum(const Frustum&) = delete;
	Frustum(Frustum&&) noexcept = delete;
	Frustum& operator=(const Frustum&) = delete;
	Frustum& operator=(Frustum&&) = delete;

	static bool Culled(const ModelMesh&);
	static bool Update();
};

#endif