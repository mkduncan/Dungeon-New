#ifndef DungeonItem_H
#define DungeonItem_H

#include <map>
#include <string>

enum ItemType
{
	NULL_ITEM,
	GOLD_COINS,
	ENTRY_KEY,
	SCROLL_UNKNOWN_A,
	SCROLL_UNKNOWN_B,
	SCROLL_UNKNOWN_C,
	SCROLL_UNKNOWN_D,
	SCROLL_UNKNOWN_NULL,
	SCROLL_IDENTIFY,
	SCROLL_TELEPORT,
	SCROLL_VISION,
	SCROLL_MAP_REVEAL,
	SCROLL_NULL,
	POTION_UNKNOWN_A,
	POTION_UNKNOWN_B,
	POTION_UNKNOWN_C,
	POTION_UNKNOWN_D,
	POTION_UNKNOWN_NULL,
	POTION_HEALTH,
	POTION_INVISIBILITY,
	POTION_EXPERIENCE,
	POTION_POISON,
	POTION_NULL,
};

class DungeonItem
{
public:
	static std::map<ItemType, std::pair<ItemType, bool>> Scrolls, Potions;
	ItemType Type, HiddenType;
	size_t Count;

	DungeonItem();
	~DungeonItem();
	DungeonItem(const ItemType, const size_t);
	DungeonItem(const ItemType, const ItemType, const size_t);
	DungeonItem(const DungeonItem&);
	DungeonItem(DungeonItem&&) noexcept;
	DungeonItem& operator=(const DungeonItem&);
	DungeonItem& operator=(DungeonItem&&) noexcept;

	static const bool CreateUnknowns();
	static const std::string ItemName(const ItemType);
};

#endif