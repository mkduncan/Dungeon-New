#include "DungeonData.hpp"
#include "DungeonMap.hpp"
#include "MathManager.hpp"
#include "TraceManager.hpp"

#define MIN_CORRIDOR_LENGTH 3
#define MAX_CORRIDOR_LENGTH 9
#define MIN_ROOM_SIZE 4
#define MAX_ROOM_SIZE 8
#define ROOM_PROBABILITY 50

std::vector<Rectangle> DungeonData::Rooms, DungeonData::Exits;
std::vector<DungeonObject> DungeonData::Tiles;
int32_t DungeonData::Width = 0, DungeonData::Height = 0;

bool DungeonData::GenerateMap(const size_t sizeX, const size_t sizeY, std::vector<std::vector<uint8_t>>& grid, size_t& tileCount)
{
	size_t offset = 0;

	grid.clear();
	tileCount = 0;

	if (!DungeonData::Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (sizeX < 32 || sizeY < 32 || sizeX > (1 << 16) || sizeY > (1 << 16))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonData::Tiles.resize(sizeX * sizeY, DungeonObject::Unused);
	DungeonData::Width = static_cast<int32_t>(sizeX);
	DungeonData::Height = static_cast<int32_t>(sizeY);

	for (size_t index = 0; index < 8; ++index)
	{
		if (DungeonData::GenerateFeatures(static_cast<size_t>(std::sqrtf(static_cast<float>(sizeX * sizeY) * 0.0625f))))
			break;

		else if(index + 1 == 8)
		{
			DungeonData::Destroy();
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);
		}
	}

	grid = std::vector<std::vector<uint8_t>>(sizeX, std::vector<uint8_t>(sizeY, DungeonObject::Floor));
	tileCount = 0;

	for (size_t indexX = 0; indexX < sizeX; ++indexX)
		for (size_t indexY = 0; indexY < sizeY; ++indexY)
		{
			grid[indexX][indexY] = DungeonData::Tiles[offset];
			++offset;

			if (grid[indexX][indexY] == DungeonObject::Floor || grid[indexX][indexY] == DungeonObject::Wall)
				++tileCount;
		}

	return true;
}

bool DungeonData::Destroy()
{
	DungeonData::Rooms.clear();
	DungeonData::Exits.clear();
	DungeonData::Tiles.clear();
	DungeonData::Width = 0;
	DungeonData::Height = 0;

	return true;
}

uint8_t DungeonData::GetTile(const int32_t x, const int32_t y)
{
	if (x < 0 || y < 0 || x >= DungeonData::Width || y >= DungeonData::Height)
		return DungeonObject::Unused;

	return DungeonData::Tiles[x + y * DungeonData::Width];
}

bool DungeonData::SetTile(const int32_t x, const int32_t y, const uint8_t tile)
{
	if (x < 0 || y < 0 || x >= DungeonData::Width || y >= DungeonData::Height)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonData::Tiles[x + y * DungeonData::Width] = static_cast<DungeonObject>(tile);
	return true;
}

bool DungeonData::GenerateFeatures(const size_t features)
{
	if (features == 0)
		return GenerateFeatures((1 << 24));

	else if (!DungeonData::PlaceRoom(DungeonData::Width >> 1, DungeonData::Height >> 1, static_cast<uint8_t>(MathManager::RandomUnsigned(0, 3), true)))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	for (size_t index = 1; index < features; ++index)
		if (!DungeonData::PlaceFeature())
			break;

	if (!DungeonData::PlaceObject(DungeonObject::AscendDoor))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!DungeonData::PlaceObject(DungeonObject::DescendDoor))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	for (size_t index = 0; index < DungeonData::Tiles.size(); ++index)
	{
		if (DungeonData::Tiles[index] == DungeonObject::Unused)
			DungeonData::Tiles[index] = DungeonObject::Floor;

		else if (DungeonData::Tiles[index] == DungeonObject::Floor || DungeonData::Tiles[index] == DungeonObject::Corridor)
			DungeonData::Tiles[index] = DungeonObject::Unused;
	}

	return true;
}

bool DungeonData::PlaceCorridor(const int32_t x, const int32_t y, const uint8_t direction)
{
	Rectangle corridor(x, y, 0, 0);

	if (MathManager::RandomBoolean())
	{
		corridor.Width = MathManager::RandomSigned(MIN_CORRIDOR_LENGTH, MAX_CORRIDOR_LENGTH);
		corridor.Height = 1;

		if (direction == 0)
		{
			corridor.Y = y - 1;

			if (MathManager::RandomBoolean())
				corridor.X = x - corridor.Width + 1;
		}

		else if (direction == 1)
		{
			corridor.Y = y + 1;

			if (MathManager::RandomBoolean())
				corridor.X = x - corridor.Width + 1;
		}

		else if (direction == 2)
			corridor.X = x - corridor.Width;

		else if (direction == 3)
			corridor.X = x + 1;
	}

	else
	{
		corridor.Width = 1;
		corridor.Height = MathManager::RandomSigned(MIN_CORRIDOR_LENGTH, MAX_CORRIDOR_LENGTH);

		if (direction == 0)
			corridor.Y = y - corridor.Height;

		else if (direction == 1)
			corridor.Y = y + 1;

		else if (direction == 2)
		{
			corridor.X = x - 1;

			if (MathManager::RandomBoolean())
				corridor.Y = y - corridor.Height + 1;
		}

		else if (direction == 3)
		{
			corridor.X = x + 1;

			if (MathManager::RandomBoolean())
				corridor.Y = y - corridor.Height + 1;
		}
	}

	if (DungeonData::PlaceRectangle(corridor, DungeonObject::Corridor))
	{
		if (direction != 0 && corridor.Width != 1)
			DungeonData::Exits.emplace_back(Rectangle(corridor.X, corridor.Y + corridor.Height, corridor.Width, 1));

		if (direction != 1 && corridor.Width != 1)
			DungeonData::Exits.emplace_back(Rectangle(corridor.X, corridor.Y - 1, corridor.Width, 1));

		if (direction != 2 && corridor.Height != 1)
			DungeonData::Exits.emplace_back(Rectangle(corridor.X + corridor.Width, corridor.Y, 1, corridor.Height));

		if (direction != 3 && corridor.Height != 1)
			DungeonData::Exits.emplace_back(Rectangle(corridor.X - 1, corridor.Y, 1, corridor.Height));

		return true;
	}

	return false;
}

bool DungeonData::PlaceFeature()
{
	size_t randomIndex = 0, randomOffset = 0;
	int32_t x = 0, y = 0;
	uint8_t directions[4] = { 0, 1, 2, 3 };

	for (size_t index = 0; index < 64; ++index)
	{
		randomIndex = MathManager::RandomUnsigned(0, 3);
		randomOffset = MathManager::RandomUnsigned(0, 3);

		if (randomIndex != randomOffset)
			std::swap(directions[randomIndex], directions[randomOffset]);
	}

	for (size_t index = 0; index < (1 << 16); ++index)
	{
		randomIndex = MathManager::RandomUnsigned(0, DungeonData::Exits.size() - 1);
		x = MathManager::RandomSigned(DungeonData::Exits[randomIndex].X, DungeonData::Exits[randomIndex].X + DungeonData::Exits[randomIndex].Width - 1);
		y = MathManager::RandomSigned(DungeonData::Exits[randomIndex].Y, DungeonData::Exits[randomIndex].Y + DungeonData::Exits[randomIndex].Height - 1);

		for (uint8_t direction = 0; direction < 4; ++direction)
			if (DungeonData::PlaceFeature(x, y, directions[direction]))
			{
				DungeonData::Exits.erase(DungeonData::Exits.begin() + randomIndex);
				return true;
			}
	}

	return false;
}

bool DungeonData::PlaceFeature(const int32_t x, const int32_t y, const uint8_t direction)
{
	int32_t dx = 0, dy = 0;

	if (direction == 0)
		dy = 1;

	else if (direction == 1)
		dy = -1;

	else if (direction == 2)
		dx = 1;

	else if (direction == 3)
		dx = -1;

	if (DungeonData::GetTile(x + dx, y + dy) != DungeonObject::Floor && DungeonData::GetTile(x + dx, y + dy) != DungeonObject::Corridor)
		return false;

	else if (MathManager::RandomSigned(1, 100) < ROOM_PROBABILITY && DungeonData::PlaceRoom(x, y, direction))
	{
		DungeonData::SetTile(x, y, DungeonObject::ClosedDoor);
		return true;
	}

	else if (DungeonData::PlaceCorridor(x, y, direction))
	{
		if (DungeonData::GetTile(x + dx, y + dy) == DungeonObject::Floor)
			DungeonData::SetTile(x, y, DungeonObject::ClosedDoor);

		else
			DungeonData::SetTile(x, y, DungeonObject::Corridor);

		return true;
	}

	return false;
}

bool DungeonData::PlaceObject(const uint8_t tile)
{
	size_t randomIndex = 0;
	int32_t x = 0, y = 0;

	if (DungeonData::Rooms.empty())
		return false;

	randomIndex = MathManager::RandomUnsigned(0, DungeonData::Rooms.size() - 1);
	x = MathManager::RandomSigned(DungeonData::Rooms[randomIndex].X + 1, DungeonData::Rooms[randomIndex].X + DungeonData::Rooms[randomIndex].Width - 2);
	y = MathManager::RandomSigned(DungeonData::Rooms[randomIndex].Y + 1, DungeonData::Rooms[randomIndex].Y + DungeonData::Rooms[randomIndex].Height - 2);

	if (DungeonData::GetTile(x, y) == DungeonObject::Floor)
	{
		DungeonData::SetTile(x, y, tile);
		DungeonData::Rooms.erase(DungeonData::Rooms.begin() + randomIndex);

		return true;
	}

	return false;
}

bool DungeonData::PlaceRectangle(const Rectangle &rectangle, const uint8_t tile)
{
	if (rectangle.X < 1 || rectangle.Y < 1 || rectangle.X + rectangle.Width > DungeonData::Width - 1 || rectangle.Y + rectangle.Height > DungeonData::Height - 1)
		return false;

	for (int32_t y = rectangle.Y; y < rectangle.Y + rectangle.Height; ++y)
		for (int32_t x = rectangle.X; x < rectangle.X + rectangle.Width; ++x)
			if (DungeonData::GetTile(x, y) != DungeonObject::Unused)
				return false;

	for (int32_t y = rectangle.Y - 1; y < rectangle.Y + rectangle.Height + 1; ++y)
		for (int32_t x = rectangle.X - 1; x < rectangle.X + rectangle.Width + 1; ++x)
		{
			if (x == rectangle.X - 1 || y == rectangle.Y - 1 || x == rectangle.X + rectangle.Width || y == rectangle.Y + rectangle.Height)
				DungeonData::SetTile(x, y, DungeonObject::Wall);

			else
				DungeonData::SetTile(x, y, tile);
		}

	return true;
}

bool DungeonData::PlaceRoom(const int32_t x, const int32_t y, const uint8_t direction, const bool firstRoom)
{
	Rectangle room(0, 0, MathManager::RandomSigned(MIN_ROOM_SIZE, MAX_ROOM_SIZE), MathManager::RandomSigned(MIN_ROOM_SIZE, MAX_ROOM_SIZE));

	if (direction == 0)
	{
		room.X = x - (room.Width >> 1);
		room.Y = y - room.Height;
	}

	else if (direction == 1)
	{
		room.X = x - (room.Width >> 1);
		room.Y = y + 1;
	}

	else if (direction == 2)
	{
		room.X = x - room.Width;
		room.Y = y - (room.Height >> 1);
	}

	else if (direction == 3)
	{
		room.X = x + 1;
		room.Y = y - (room.Height >> 1);
	}

	if (!DungeonData::PlaceRectangle(room, DungeonObject::Floor))
		return false;

	else
	{
		DungeonData::Rooms.emplace_back(room);

		if (direction != 0 || firstRoom)
			DungeonData::Exits.emplace_back(Rectangle(room.X, room.Y + room.Height, room.Width, 1));

		if (direction != 1 || firstRoom)
			DungeonData::Exits.emplace_back(Rectangle(room.X, room.Y - 1, room.Width, 1));

		if (direction != 2 || firstRoom)
			DungeonData::Exits.emplace_back(Rectangle(room.X + room.Width, room.Y, 1, room.Height));

		if (direction != 3 || firstRoom)
			DungeonData::Exits.emplace_back(Rectangle(room.X - 1, room.Y, 1, room.Height));
	}

	return true;
}