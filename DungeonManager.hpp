#ifndef DungeonManager_H
#define DungeonManager_H

#include "AlignedBox.hpp"
#include "Model.hpp"

#include <map>

class DungeonManager
{
public:
	static std::map<std::string, Model> Models;
	static std::map<LightType, std::vector<Light>> Lights;
	static std::vector<AlignedBox> Bounds;
	static size_t Level;

	DungeonManager() = delete;
	~DungeonManager() = delete;
	DungeonManager(const DungeonManager&) = delete;
	DungeonManager(DungeonManager&&) noexcept = delete;
	DungeonManager& operator=(const DungeonManager&) = delete;
	DungeonManager& operator=(DungeonManager&&) noexcept = delete;

public:
	static bool Create(const size_t = 0);
	static bool Update();
	static bool Render();
	static bool Destroy();
};

#endif