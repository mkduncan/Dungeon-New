#include "Camera.hpp"
#include "DungeonModel.hpp"
#include "Frustum.hpp"

#include <glm/glm.hpp>

//	Based on code from "https://gist.github.com/podgorskiy/e698d18879588ada9014768e3e82a644"
//	as written by user: "podgorskiy"; also known as: "Stanislav Pidhorskyi".

enum Planes { Left = 0, Right, Bottom, Top, Near, Far, Count, Combinations = Count * (Count - 1) / 2 };
template<Planes i, Planes j> struct Indexer { enum { k = i * (9 - i) / 2 + j - 1 }; };

glm::vec4 Frustum::Plane[6];
glm::vec3 Frustum::Point[8];

template<Planes a, Planes b, Planes c> static glm::vec3 Intersection(const glm::vec3* crosses)
{
	glm::vec3 result = glm::mat3(crosses[Indexer<b, c>::k], -crosses[Indexer<a, c>::k], crosses[Indexer<a, b>::k]) * glm::vec3(Frustum::Plane[a].w, Frustum::Plane[b].w, Frustum::Plane[c].w);
	float delta = -1.0f / glm::dot(glm::vec3(Frustum::Plane[a]), crosses[Indexer<b, c>::k]);

	return result * delta;
}

bool Frustum::Culled(const ModelMesh& model)
{
	glm::vec4 offsetMinimum = model.Transform * glm::vec4(model.Bounds.Minimum, 1.0f), offsetMaximum = model.Transform * glm::vec4(model.Bounds.Maximum, 1.0f);
	glm::vec3 minimum = glm::min(offsetMinimum, offsetMaximum), maximum = glm::max(offsetMinimum, offsetMaximum);
	size_t outside = 0;

	if (glm::distance(Camera::Position, glm::clamp(Camera::Position, minimum, maximum)) < DungeonModel::MeshScale)
		return false;

	for (size_t index = 0; index < Count; ++index)
	{
		if (glm::dot(Frustum::Plane[index], glm::vec4(minimum.x, minimum.y, minimum.z, 1.0f)) < 0.0f &&
			glm::dot(Frustum::Plane[index], glm::vec4(maximum.x, minimum.y, minimum.z, 1.0f)) < 0.0f &&
			glm::dot(Frustum::Plane[index], glm::vec4(minimum.x, maximum.y, minimum.z, 1.0f)) < 0.0f &&
			glm::dot(Frustum::Plane[index], glm::vec4(maximum.x, maximum.y, minimum.z, 1.0f)) < 0.0f &&
			glm::dot(Frustum::Plane[index], glm::vec4(minimum.x, minimum.y, maximum.z, 1.0f)) < 0.0f &&
			glm::dot(Frustum::Plane[index], glm::vec4(maximum.x, minimum.y, maximum.z, 1.0f)) < 0.0f &&
			glm::dot(Frustum::Plane[index], glm::vec4(minimum.x, maximum.y, maximum.z, 1.0f)) < 0.0f &&
			glm::dot(Frustum::Plane[index], glm::vec4(maximum.x, maximum.y, maximum.z, 1.0f)) < 0.0f)
		{
			return true;
		}
	}

	for (size_t index = 0; index < 8; ++index)
		outside += ((Frustum::Point[index].x > maximum.x) ? 1 : 0);
	
	if (outside == 8)
		return true;

	outside = 0;

	for (size_t index = 0; index < 8; ++index)
		outside += ((Frustum::Point[index].x < minimum.x) ? 1 : 0);

	if (outside == 8)
		return true;

	outside = 0;
	
	for (size_t index = 0; index < 8; ++index)
		outside += ((Frustum::Point[index].y > maximum.y) ? 1 : 0);

	if (outside == 8)
		return true;

	outside = 0;

	for (size_t index = 0; index < 8; ++index)
		outside += ((Frustum::Point[index].y < minimum.y) ? 1 : 0);

	if (outside == 8)
		return true;

	outside = 0;

	for (size_t index = 0; index < 8; ++index)
		outside += ((Frustum::Point[index].z > maximum.z) ? 1 : 0);

	if (outside == 8)
		return true;
	
	outside = 0;

	for (size_t index = 0; index < 8; ++index)
		outside += ((Frustum::Point[index].z < minimum.z) ? 1 : 0);

	if (outside == 8)
		return true;

	return false;
}

bool Frustum::Update()
{
	glm::mat4 matrix = glm::transpose(Camera::Projection * Camera::View);

	Frustum::Plane[Left] = matrix[3] + matrix[0];
	Frustum::Plane[Right] = matrix[3] - matrix[0];
	Frustum::Plane[Bottom] = matrix[3] + matrix[1];
	Frustum::Plane[Top] = matrix[3] - matrix[1];
	Frustum::Plane[Near] = matrix[3] + matrix[2];
	Frustum::Plane[Far] = matrix[3] - matrix[2];

	glm::vec3 crosses[Combinations] =
	{
		glm::cross(glm::vec3(Frustum::Plane[Left]), glm::vec3(Frustum::Plane[Right])),
		glm::cross(glm::vec3(Frustum::Plane[Left]), glm::vec3(Frustum::Plane[Bottom])),
		glm::cross(glm::vec3(Frustum::Plane[Left]), glm::vec3(Frustum::Plane[Top])),
		glm::cross(glm::vec3(Frustum::Plane[Left]), glm::vec3(Frustum::Plane[Near])),
		glm::cross(glm::vec3(Frustum::Plane[Left]), glm::vec3(Frustum::Plane[Far])),
		glm::cross(glm::vec3(Frustum::Plane[Right]), glm::vec3(Frustum::Plane[Bottom])),
		glm::cross(glm::vec3(Frustum::Plane[Right]), glm::vec3(Frustum::Plane[Top])),
		glm::cross(glm::vec3(Frustum::Plane[Right]), glm::vec3(Frustum::Plane[Near])),
		glm::cross(glm::vec3(Frustum::Plane[Right]), glm::vec3(Frustum::Plane[Far])),
		glm::cross(glm::vec3(Frustum::Plane[Bottom]), glm::vec3(Frustum::Plane[Top])),
		glm::cross(glm::vec3(Frustum::Plane[Bottom]), glm::vec3(Frustum::Plane[Near])),
		glm::cross(glm::vec3(Frustum::Plane[Bottom]), glm::vec3(Frustum::Plane[Far])),
		glm::cross(glm::vec3(Frustum::Plane[Top]), glm::vec3(Frustum::Plane[Near])),
		glm::cross(glm::vec3(Frustum::Plane[Top]), glm::vec3(Frustum::Plane[Far])),
		glm::cross(glm::vec3(Frustum::Plane[Near]), glm::vec3(Frustum::Plane[Far]))
	};

	Frustum::Point[0] = Intersection<Left, Bottom, Near>(crosses);
	Frustum::Point[1] = Intersection<Left, Top, Near>(crosses);
	Frustum::Point[2] = Intersection<Right, Bottom, Near>(crosses);
	Frustum::Point[3] = Intersection<Right, Top, Near>(crosses);
	Frustum::Point[4] = Intersection<Left, Bottom, Far>(crosses);
	Frustum::Point[5] = Intersection<Left, Top, Far>(crosses);
	Frustum::Point[6] = Intersection<Right, Bottom, Far>(crosses);
	Frustum::Point[7] = Intersection<Right, Top, Far>(crosses);

	return true;
}