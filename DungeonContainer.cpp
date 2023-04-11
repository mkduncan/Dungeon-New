#include "DungeonContainer.hpp"
#include "DungeonItem.hpp"
#include "DungeonMap.hpp"
#include "Logger.hpp"

DungeonContainer::DungeonContainer() : Items(), Object()
{
}

DungeonContainer::~DungeonContainer()
{
	this->Items.clear();
}

DungeonContainer::DungeonContainer(const DungeonContainer& container) : Items(container.Items), Object(container.Object)
{
}

DungeonContainer::DungeonContainer(DungeonContainer&& container) noexcept : Items(std::move(container.Items)), Object(std::move(container.Object))
{
}

DungeonContainer& DungeonContainer::operator=(const DungeonContainer& container)
{
	this->Items = container.Items;
	this->Object = container.Object;

	return (*this);
}

DungeonContainer& DungeonContainer::operator=(DungeonContainer&& container) noexcept
{
	this->Items = std::move(container.Items);
	this->Object = std::move(container.Object);

	return (*this);
}

const bool DungeonContainer::CreateCommon()
{
	ItemType unknownPotion = ItemType::NULL_ITEM, unknownScroll = ItemType::NULL_ITEM;
	size_t itemIndex = 0;

	if (!this->Items.empty())
		return true;

	else if (DungeonItem::Scrolls.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (DungeonItem::Potions.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	this->Items.resize(4, DungeonItem());

	if (DungeonMap::RandomBoolean(0.75f))
	{
		this->Items[itemIndex] = DungeonItem(ItemType::GOLD_COINS, DungeonMap::RandomUnsigned(1, 10));
		++itemIndex;
	}

	if (DungeonMap::RandomBoolean(0.125f))
	{
		unknownPotion = static_cast<ItemType>(DungeonMap::RandomUnsigned(ItemType::POTION_UNKNOWN_A, ItemType::POTION_UNKNOWN_NULL - 1));

		if (DungeonItem::Potions[unknownPotion].second)
			this->Items[itemIndex] = DungeonItem(DungeonItem::Potions[unknownPotion].first, 1);

		else
			this->Items[itemIndex] = DungeonItem(unknownPotion, DungeonItem::Potions[unknownPotion].first, 1);

		++itemIndex;
	}

	if (DungeonMap::RandomBoolean(0.125f))
	{
		unknownScroll = static_cast<ItemType>(DungeonMap::RandomUnsigned(ItemType::SCROLL_UNKNOWN_A, ItemType::SCROLL_UNKNOWN_NULL - 1));

		if (DungeonItem::Scrolls[unknownScroll].second)
			this->Items[itemIndex] = DungeonItem(DungeonItem::Scrolls[unknownScroll].first, 1);

		else
			this->Items[itemIndex] = DungeonItem(unknownScroll, DungeonItem::Scrolls[unknownScroll].first, 1);

		++itemIndex;
	}

	return true;
}

const bool DungeonContainer::CreateInventory()
{
	if (!this->Items.empty())
		return true;

	else if (DungeonItem::Scrolls.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (DungeonItem::Potions.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	this->Items.resize(16, DungeonItem());
	this->Items[0] = DungeonItem(ItemType::GOLD_COINS, DungeonMap::RandomUnsigned(10, 25));
	this->Items[1] = DungeonItem(ItemType::POTION_HEALTH, 1);

	return true;
}

const bool DungeonContainer::CreateTreasure()
{
	ItemType unknownPotion = ItemType::NULL_ITEM, unknownScroll = ItemType::NULL_ITEM;
	size_t itemIndex = 0;

	if (!this->Items.empty())
		return true;

	else if (DungeonItem::Scrolls.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (DungeonItem::Potions.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	this->Items.resize(9, DungeonItem());
	this->Items[itemIndex] = DungeonItem(ItemType::GOLD_COINS, DungeonMap::RandomUnsigned(25, 50));
	++itemIndex;

	if (DungeonMap::RandomBoolean(0.5f))
	{
		unknownPotion = static_cast<ItemType>(DungeonMap::RandomUnsigned(ItemType::POTION_UNKNOWN_A, ItemType::POTION_UNKNOWN_NULL - 1));

		if (DungeonItem::Potions[unknownPotion].second)
			this->Items[itemIndex] = DungeonItem(DungeonItem::Potions[unknownPotion].first, 1);

		else
			this->Items[itemIndex] = DungeonItem(unknownPotion, DungeonItem::Potions[unknownPotion].first, 1);

		++itemIndex;

		if (DungeonMap::RandomBoolean(0.25f))
		{
			unknownPotion = static_cast<ItemType>(DungeonMap::RandomUnsigned(ItemType::POTION_UNKNOWN_A, ItemType::POTION_UNKNOWN_NULL - 1));

			if (unknownPotion != this->Items[itemIndex - 1].Type)
			{
				if (DungeonItem::Potions[unknownPotion].second)
					this->Items[itemIndex] = DungeonItem(DungeonItem::Potions[unknownPotion].first, 1);

				else
					this->Items[itemIndex] = DungeonItem(unknownPotion, DungeonItem::Potions[unknownPotion].first, 1);

				++itemIndex;
			}

			else
				++(this->Items[itemIndex - 1].Count);
		}
	}

	if (DungeonMap::RandomBoolean(0.5f))
	{
		unknownScroll = static_cast<ItemType>(DungeonMap::RandomUnsigned(ItemType::SCROLL_UNKNOWN_A, ItemType::SCROLL_UNKNOWN_NULL - 1));

		if (DungeonItem::Scrolls[unknownScroll].second)
			this->Items[itemIndex] = DungeonItem(DungeonItem::Scrolls[unknownScroll].first, 1);

		else
			this->Items[itemIndex] = DungeonItem(unknownScroll, DungeonItem::Scrolls[unknownScroll].first, 1);

		++itemIndex;

		if (DungeonMap::RandomBoolean(0.25f))
		{
			unknownScroll = static_cast<ItemType>(DungeonMap::RandomUnsigned(ItemType::SCROLL_UNKNOWN_A, ItemType::SCROLL_UNKNOWN_NULL - 1));

			if (unknownScroll != this->Items[itemIndex - 1].Type)
			{
				if (DungeonItem::Scrolls[unknownScroll].second)
					this->Items[itemIndex] = DungeonItem(DungeonItem::Scrolls[unknownScroll].first, 1);

				else
					this->Items[itemIndex] = DungeonItem(unknownScroll, DungeonItem::Scrolls[unknownScroll].first, 1);

				++itemIndex;
			}

			else
				++(this->Items[itemIndex - 1].Count);
		}
	}

	return true;
}