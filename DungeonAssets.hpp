#ifndef DungeonAssets_H
#define DungeonAssets_H

#include <map>
#include <string>

enum AssetType
{
	NULL_ASSET = 0,
	MODEL_ASSET = 1,
	IMAGE_ASSET = 2,
};

class DungeonAssets
{
public:
	static std::map<std::string, std::pair<AssetType, std::string>> Assets;

	DungeonAssets() = delete;
	~DungeonAssets() = delete;
	DungeonAssets(const DungeonAssets&) = delete;
	DungeonAssets(DungeonAssets&&) noexcept = delete;
	DungeonAssets& operator=(const DungeonAssets&) = delete;
	DungeonAssets& operator=(DungeonAssets&&) noexcept = delete;

	static const bool Load();
};

#endif