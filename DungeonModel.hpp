#ifndef DungeonModel_H
#define DungeonModel_H

#include "ModelMesh.hpp"

class DungeonModel
{
public:
	static float MeshScale, MeshHeight, TextureScale;

	DungeonModel() = delete;
	~DungeonModel() = delete;
	DungeonModel(const DungeonModel&) = delete;
	DungeonModel(DungeonModel&&) noexcept = delete;
	DungeonModel& operator=(const DungeonModel&) = delete;
	DungeonModel& operator=(DungeonModel&&) noexcept = delete;

	static const bool Generate(const size_t, const size_t);
	static const bool CollideCamera();
	static const bool PlaceCamera();
};

#endif