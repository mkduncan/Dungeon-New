#ifndef DungeonScene_H
#define DungeonScene_H

#include "DungeonContainer.hpp"
#include "DungeonObject.hpp"

class DungeonScene
{
public:
	static std::vector<DungeonObject> Objects;
	static std::vector<DungeonContainer> Containers;
	static std::vector<Light> Lights;

	DungeonScene() = delete;
	~DungeonScene() = delete;
	DungeonScene(const DungeonScene&) = delete;
	DungeonScene(DungeonScene&&) noexcept = delete;
	DungeonScene& operator=(const DungeonScene&) = delete;
	DungeonScene& operator=(DungeonScene&&) noexcept = delete;

	static const bool Destroy();
	static const bool Generate(const size_t);
	static const bool Initialize();
	static const bool Render();
	static const bool Update();

};

#endif