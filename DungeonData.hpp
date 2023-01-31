#ifndef DungeonData_H
#define DungeonData_H

#include "Rectangle.hpp"

#include <cstdint>
#include <vector>

enum DungeonObject
{
	Unused = ' ',
	Wall = '#',
	ClosedDoor = '+',
	Corridor = ',',
	OpenedDoor = '-',
	Floor = '.',
	AscendDoor = '<',
	DescendDoor = '>',
	Barrel = 'B',
	Chest = 'C',
	Enemy = 'E',
};

class DungeonData
{
private:
	static std::vector<Rectangle> Rooms, Exits;
	static std::vector<DungeonObject> Tiles;
	static int32_t Width, Height;

	DungeonData() = delete;
	~DungeonData() = delete;
	DungeonData(const DungeonData&) = delete;
	DungeonData(DungeonData&&) = delete;
	DungeonData& operator=(const DungeonData&) = delete;
	DungeonData& operator=(DungeonData&&) = delete;

	static uint8_t GetTile(const int32_t, const int32_t);
	static bool SetTile(const int32_t, const int32_t, const uint8_t);

	static bool GenerateFeatures(const size_t = 0);
	static bool PlaceCorridor(const int32_t, const int32_t, const uint8_t);
	static bool PlaceFeature();
	static bool PlaceFeature(const int32_t, const int32_t, const uint8_t);
	static bool PlaceObject(const uint8_t);
	static bool PlaceRectangle(const Rectangle&, const uint8_t);
	static bool PlaceRoom(const int32_t, const int32_t, const uint8_t, const bool = false);

	friend class DungeonManager;
	friend class DungeonMap;

public:
	static bool GenerateMap(const size_t, const size_t, std::vector<std::vector<uint8_t>>&, size_t&);
	static bool Destroy();
};

#endif
