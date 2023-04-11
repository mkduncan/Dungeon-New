#ifndef DungeonMap_H
#define DungeonMap_H

#include "Rectangle.hpp"

#include <random>
#include <string>
#include <vector>

enum DungeonTile
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

class DungeonMap
{
private:
	static std::vector<Rectangle> Rooms, Exits;
	static std::vector<uint8_t> Tiles;
	static std::mt19937_64 Random;

	DungeonMap() = delete;
	~DungeonMap() = delete;
	DungeonMap(const DungeonMap&) = delete;
	DungeonMap(DungeonMap&&) = delete;
	DungeonMap& operator=(const DungeonMap&) = delete;
	DungeonMap& operator=(DungeonMap&&) = delete;

	static const float Distance(const std::pair<size_t, size_t>&, const std::pair<size_t, size_t>&);
	static const bool EraseMaxRectangle(const size_t, const size_t, const size_t, const size_t);
	static const size_t FindMaxRectangle(size_t&, size_t&, size_t&, size_t&);
	static const bool GenerateFeatures(const size_t = 0);
	static const bool GenerateMap(const size_t, const size_t, size_t&);
	static const uint8_t GetTile(const int32_t, const int32_t);
	static const bool IsRoomCorner(const size_t, const size_t);
	static const bool IsCorridorEnd(const size_t, const size_t);
	static const bool IsIsolated(const size_t, const size_t);
	static const bool PlaceCorridor(const int32_t, const int32_t, const uint8_t);
	static const bool PlaceFeature();
	static const bool PlaceFeature(const int32_t, const int32_t, const uint8_t);
	static const bool PlaceObject(const uint8_t);
	static const bool PlaceObjects();
	static const bool PlaceRectangle(const Rectangle&, const uint8_t);
	static const bool PlaceRoom(const int32_t, const int32_t, const uint8_t, const bool = false);
	static const bool SetTile(const int32_t, const int32_t, const uint8_t);

public:
	static std::vector<std::vector<uint8_t>> Map;
	static std::vector<Rectangle> Bounds;
	static size_t Seed;
	static int32_t Width, Height;

	static const bool Destroy();
	static const bool Generate(const size_t, const size_t, const size_t);
	static const bool Generate(const size_t, const size_t);
	static const bool WriteImage(const std::string&);

	static const bool RandomBoolean(const float = 0.5f);
	static const int32_t RandomSigned(const int32_t, const int32_t);
	static const size_t RandomUnsigned(const size_t, const size_t);
};

#endif