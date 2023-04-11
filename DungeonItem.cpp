#include "DungeonItem.hpp"
#include "DungeonMap.hpp"
#include "Logger.hpp"

#include <algorithm>
#include <vector>

std::map<ItemType, std::pair<ItemType, bool>> DungeonItem::Scrolls, DungeonItem::Potions;

DungeonItem::DungeonItem() : Type(ItemType::NULL_ITEM), HiddenType(ItemType::NULL_ITEM), Count(0)
{
}

DungeonItem::~DungeonItem()
{
}

DungeonItem::DungeonItem(const ItemType type, const size_t count) : Type(type), HiddenType(type), Count(count)
{
}

DungeonItem::DungeonItem(const ItemType type, const ItemType hiddenType, const size_t count) : Type(type), HiddenType(hiddenType), Count(count)
{
}

DungeonItem::DungeonItem(const DungeonItem& item) : Type(item.Type), HiddenType(item.HiddenType), Count(item.Count)
{
}

DungeonItem::DungeonItem(DungeonItem&& item) noexcept : Type(item.Type), HiddenType(item.HiddenType), Count(item.Count)
{
	item.Type = ItemType::NULL_ITEM;
	item.HiddenType = ItemType::NULL_ITEM;
	item.Count = 0;
}

DungeonItem& DungeonItem::operator=(const DungeonItem& item)
{
	this->Type = item.Type;
	this->HiddenType = item.HiddenType;
	this->Count = item.Count;

	return (*this);
}

DungeonItem& DungeonItem::operator=(DungeonItem&& item) noexcept
{
	this->Type = item.Type;
	this->HiddenType = item.HiddenType;
	this->Count = item.Count;
	item.Type = ItemType::NULL_ITEM;
	item.HiddenType = ItemType::NULL_ITEM;
	item.Count = 0;

	return (*this);
}

const bool DungeonItem::CreateUnknowns()
{
	std::vector<ItemType> scrolls, potions;

	DungeonItem::Scrolls.clear();
	DungeonItem::Potions.clear();

	if ((ItemType::POTION_UNKNOWN_NULL - ItemType::POTION_UNKNOWN_A) != (ItemType::POTION_NULL - ItemType::POTION_HEALTH))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if ((ItemType::SCROLL_UNKNOWN_NULL - ItemType::SCROLL_UNKNOWN_A) != (ItemType::SCROLL_NULL - ItemType::SCROLL_IDENTIFY))
		return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t item = ItemType::SCROLL_UNKNOWN_A; item < ItemType::SCROLL_UNKNOWN_NULL; ++item)
		scrolls.push_back(static_cast<ItemType>(item));

	for (size_t index = 0; index < (1 << 8); ++index)
		std::swap(scrolls[DungeonMap::RandomUnsigned(0, scrolls.size() - 1)], scrolls[DungeonMap::RandomUnsigned(0, scrolls.size() - 1)]);

	for (size_t index = 0; index < scrolls.size(); ++index)
		DungeonItem::Scrolls[scrolls[index]] = std::make_pair(static_cast<ItemType>(ItemType::SCROLL_IDENTIFY + index), false);

	for (size_t item = ItemType::POTION_UNKNOWN_A; item < ItemType::POTION_UNKNOWN_NULL; ++item)
		potions.push_back(static_cast<ItemType>(item));

	for (size_t index = 0; index < (1 << 8); ++index)
		std::swap(potions[DungeonMap::RandomUnsigned(0, potions.size() - 1)], potions[DungeonMap::RandomUnsigned(0, potions.size() - 1)]);

	for (size_t index = 0; index < potions.size(); ++index)
		DungeonItem::Potions[potions[index]] = std::make_pair(static_cast<ItemType>(ItemType::POTION_HEALTH + index), false);

	return true;
}

const std::string DungeonItem::ItemName(const ItemType type)
{
	std::string name;

	switch (type)
	{
	case GOLD_COINS: name = "Coins"; break;
	case ENTRY_KEY: name = "Key"; break;
	case SCROLL_IDENTIFY: name = "Scroll_Identify"; break;
	case SCROLL_TELEPORT: name = "Scroll_Teleport"; break;
	case SCROLL_VISION: name = "Scroll_Vision"; break;
	case SCROLL_MAP_REVEAL: name = "Scroll_Map_Reveal"; break;
	case POTION_HEALTH: name = "Potion_Health"; break;
	case POTION_INVISIBILITY: name = "Potion_Invisibility"; break;
	case POTION_EXPERIENCE: name = "Potion_Experience"; break;
	case POTION_POISON: name = "Potion_Poison"; break;
	}

	if (name.empty())
	{
		if (type >= ItemType::SCROLL_UNKNOWN_A && type < ItemType::SCROLL_UNKNOWN_NULL)
			name = "Scroll_Unknown";

		else if (type >= ItemType::POTION_UNKNOWN_A && type < ItemType::POTION_UNKNOWN_NULL)
			name = "Potion_Unknown";
	}

	return name;
}