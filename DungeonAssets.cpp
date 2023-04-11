#include "Assets.hpp"
#include "DungeonAssets.hpp"
#include "Logger.hpp"

std::map<std::string, std::pair<AssetType, std::string>> DungeonAssets::Assets
{
	{ "WoodenLadderUp", { AssetType::MODEL_ASSET, "Assets/ladder.obj" } },
	{ "WoodenLadderDown", { AssetType::MODEL_ASSET, "Assets/ladder_down.obj" } },
	{ "WoodenDoor", { AssetType::MODEL_ASSET, "Assets/door.obj" } },
	{ "WoodenBarrel", { AssetType::MODEL_ASSET, "Assets/barrel.obj" } },
	{ "WoodenChest", { AssetType::MODEL_ASSET, "Assets/chest.obj" } },
	{ "Inventory", { AssetType::IMAGE_ASSET, "Assets/Inventory.png" } },
	{ "Inventory_Common", { AssetType::IMAGE_ASSET, "Assets/Inventory_Common.png" } },
	{ "Inventory_Rare", { AssetType::IMAGE_ASSET, "Assets/Inventory_Rare.png" } },
	{ "Inventory_Selected", { AssetType::IMAGE_ASSET, "Assets/Inventory_Selected.png" } },
	{ "Potion_Unknown", { AssetType::IMAGE_ASSET, "Assets/Potion_Unknown.png" } },
	{ "Potion_Health", { AssetType::IMAGE_ASSET, "Assets/Potion_Health.png" } },
	{ "Potion_Invisibility", { AssetType::IMAGE_ASSET, "Assets/Potion_Invisibility.png" } },
	{ "Potion_Experience", { AssetType::IMAGE_ASSET, "Assets/Potion_Experience.png" } },
	{ "Potion_Poison", { AssetType::IMAGE_ASSET, "Assets/Potion_Poison.png" } },
	{ "Coins", { AssetType::IMAGE_ASSET, "Assets/Coins.png" } },
	{ "Key", { AssetType::IMAGE_ASSET, "Assets/Key.png" } },
	{ "Scroll_Unknown", { AssetType::IMAGE_ASSET, "Assets/Scroll_Unknown.png" } },
	{ "Scroll_Identify", { AssetType::IMAGE_ASSET, "Assets/Scroll_Identify.png" } },
	{ "Scroll_Teleport", { AssetType::IMAGE_ASSET, "Assets/Scroll_Teleport.png" } },
	{ "Scroll_Vision", { AssetType::IMAGE_ASSET, "Assets/Scroll_Vision.png" } },
	{ "Scroll_Map_Reveal", { AssetType::IMAGE_ASSET, "Assets/Scroll_Map_Reveal.png" } },
};

const bool DungeonAssets::Load()
{
	for (std::map<std::string, std::pair<AssetType, std::string>>::const_iterator iterator = DungeonAssets::Assets.begin(); iterator != DungeonAssets::Assets.end(); ++iterator)
	{
		if (iterator->second.first == AssetType::NULL_ASSET)
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (iterator->second.first == AssetType::MODEL_ASSET && !Assets::LoadModel(iterator->first, iterator->second.second))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (iterator->second.first == AssetType::IMAGE_ASSET && !Assets::LoadImage(iterator->first, iterator->second.second))
			return Logger::LogError(__FUNCTION__, __LINE__);
	}

	return true;
}