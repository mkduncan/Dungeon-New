#ifndef DungeonMap_H
#define DungeonMap_H

#include "AlignedBox.hpp"
#include "Model.hpp"

class DungeonMap
{
public:
	static std::vector<std::vector<uint8_t>> Map;
	static float MeshScale, MeshHeight, TextureScale;
	static bool InitializeCamera;

	DungeonMap() = delete;
	~DungeonMap() = delete;
	DungeonMap(const DungeonMap&) = delete;
	DungeonMap(DungeonMap&&) noexcept = delete;
	DungeonMap& operator=(const DungeonMap&) = delete;
	DungeonMap& operator=(DungeonMap&&) noexcept = delete;

	friend class DungeonData;
	friend class DungeonDebugger;
	friend class DungeonManager;

public:
	static bool GenerateModel(const size_t, const size_t, Model&, std::vector<AlignedBox>&);
	static bool BoundCamera();
};

#endif