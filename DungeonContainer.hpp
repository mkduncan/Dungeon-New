#ifndef DungeonContainer_H
#define DungeonContainer_H

#include "DungeonItem.hpp"
#include "DungeonObject.hpp"

#include <vector>

class DungeonContainer
{
public:
	std::vector<DungeonItem> Items;
	DungeonObject Object;

	DungeonContainer();
	~DungeonContainer();
	DungeonContainer(const DungeonContainer&);
	DungeonContainer(DungeonContainer&&) noexcept;
	DungeonContainer& operator=(const DungeonContainer&);
	DungeonContainer& operator=(DungeonContainer&&) noexcept;

	const bool CreateCommon();
	const bool CreateInventory();
	const bool CreateTreasure();
};

#endif