#include "Camera.hpp"
#include "DungeonMap.hpp"
#include "DungeonModel.hpp"
#include "Logger.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <stb/stb_image_write.h>

#define MIN_CORRIDOR_LENGTH 3
#define MAX_CORRIDOR_LENGTH 7
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 8
#define ROOM_PROBABILITY 67

std::vector<std::vector<uint8_t>> DungeonMap::Map;
std::vector<Rectangle> DungeonMap::Rooms, DungeonMap::Exits, DungeonMap::Bounds;
std::vector<uint8_t> DungeonMap::Tiles;
std::mt19937_64 DungeonMap::Random(std::chrono::high_resolution_clock::now().time_since_epoch().count());
size_t DungeonMap::Seed = 0;
int32_t DungeonMap::Width = 0, DungeonMap::Height = 0;

const bool DungeonMap::Destroy()
{
	DungeonMap::Map.clear();
	DungeonMap::Rooms.clear();
	DungeonMap::Exits.clear();
	DungeonMap::Bounds.clear();
	DungeonMap::Tiles.clear();

	return true;
}

const bool DungeonMap::Generate(const size_t sizeX, const size_t sizeY, const size_t tries)
{
	for (size_t index = 0; index < tries; ++index)
		if (DungeonMap::Generate(sizeX, sizeY))
			return true;

	return false;
}

const bool DungeonMap::Generate(const size_t sizeX, const size_t sizeY)
{
	std::vector<std::vector<uint8_t>> previousMap;
	size_t tileCount = 0, indexX = 0, indexY = 0, countX = 0, countY = 0, maxArea = 1;

	if (!DungeonMap::Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (sizeX < 32 || sizeY < 32 || sizeX > 65536 || sizeY > 65536)
		return Logger::LogError(__FUNCTION__, __LINE__);

	DungeonMap::Seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	DungeonMap::Random = std::mt19937_64(DungeonMap::Seed);

	if (!DungeonMap::GenerateMap(sizeX, sizeY, tileCount))
	{
		DungeonMap::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	previousMap = DungeonMap::Map;

	while (tileCount > 0 && maxArea != 0 && tileCount <= sizeX * sizeY)
	{
		maxArea = DungeonMap::FindMaxRectangle(indexX, indexY, countX, countY);
		tileCount -= maxArea;

		if (!DungeonMap::EraseMaxRectangle(indexX, indexY, countX, countY))
			return Logger::LogError(__FUNCTION__, __LINE__);

		DungeonMap::Bounds.push_back(Rectangle(static_cast<int32_t>(indexX), static_cast<int32_t>(indexY), static_cast<int32_t>(countX), static_cast<int32_t>(countY)));
	}

	if (tileCount != 0)
	{
		DungeonMap::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	DungeonMap::Map = std::move(previousMap);

	if (!DungeonMap::PlaceObjects())
	{
		DungeonMap::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	return true;
}

const bool DungeonMap::WriteImage(const std::string& imageFile)
{
	std::vector<std::vector<uint8_t>> imageBuffer;
	std::vector<uint8_t> imageData;
	size_t index = 0, indexX = 0, indexY = 0;

	if (DungeonMap::Map.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (imageFile.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	imageBuffer = std::vector<std::vector<uint8_t>>(DungeonMap::Width, std::vector<uint8_t>(static_cast<size_t>(DungeonMap::Height) * 3, 0));

	for (indexX = 0; indexX < DungeonMap::Width; ++indexX)
		for (indexY = 0; indexY < DungeonMap::Height; ++indexY)
		{
			if (DungeonMap::Map[indexX][indexY] == DungeonTile::Unused || DungeonMap::Map[indexX][indexY] == DungeonTile::Corridor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 90;
				imageBuffer[indexX][indexY * 3 + 1] = 60;
				imageBuffer[indexX][indexY * 3 + 2] = 40;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::Wall)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 160;
				imageBuffer[indexX][indexY * 3 + 1] = 160;
				imageBuffer[indexX][indexY * 3 + 2] = 160;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::AscendDoor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 104;
				imageBuffer[indexX][indexY * 3 + 2] = 104;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::DescendDoor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 0;
				imageBuffer[indexX][indexY * 3 + 2] = 0;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::ClosedDoor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 76;
				imageBuffer[indexX][indexY * 3 + 1] = 255;
				imageBuffer[indexX][indexY * 3 + 2] = 0;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::Barrel)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 255;
				imageBuffer[indexX][indexY * 3 + 2] = 255;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::Chest)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 255;
				imageBuffer[indexX][indexY * 3 + 2] = 0;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::Enemy)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 0;
				imageBuffer[indexX][indexY * 3 + 1] = 0;
				imageBuffer[indexX][indexY * 3 + 2] = 255;
			}
		}

	indexX = static_cast<size_t>(Camera::Position.x / DungeonModel::MeshScale);
	indexY = static_cast<size_t>(Camera::Position.z / DungeonModel::MeshScale);

	if (indexX > 0 && indexX < DungeonMap::Width && indexY > 0 && indexY < DungeonMap::Height)
	{
		imageBuffer[indexX][indexY * 3 + 0] = 174;
		imageBuffer[indexX][indexY * 3 + 1] = 0;
		imageBuffer[indexX][indexY * 3 + 2] = 255;
	}

	imageData = std::vector<uint8_t>(imageBuffer.size() * imageBuffer[0].size());

	for (indexX = 0; indexX < imageBuffer.size(); ++indexX)
		for (indexY = 0; indexY < imageBuffer[0].size(); ++indexY)
		{
			imageData[index] = imageBuffer[indexX][indexY];
			++index;
		}

	stbi_flip_vertically_on_write(1);

	if (stbi_write_png(imageFile.c_str(), static_cast<int32_t>(DungeonMap::Width), static_cast<int32_t>(DungeonMap::Height), 3, &imageData[0], static_cast<int32_t>(DungeonMap::Width) * 3) == 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool DungeonMap::RandomBoolean(const float probability)
{
	std::bernoulli_distribution distribution(probability);
	return distribution(DungeonMap::Random);
}

const int32_t DungeonMap::RandomSigned(const int32_t minimum, const int32_t maximum)
{
	std::uniform_int_distribution<int32_t> distribution(minimum, maximum);
	return distribution(DungeonMap::Random);
}

const size_t DungeonMap::RandomUnsigned(const size_t minimum, const size_t maximum)
{
	std::uniform_int_distribution<size_t> distribution(minimum, maximum);
	return distribution(DungeonMap::Random);
}

const float DungeonMap::Distance(const std::pair<size_t, size_t>& x, const std::pair<size_t, size_t>& y)
{
	const float dx = static_cast<float>(x.first) - static_cast<float>(y.first), dy = static_cast<float>(x.second) - static_cast<float>(y.second);
	return std::sqrtf(dx * dx + dy * dy);
}

const size_t DungeonMap::FindMaxRectangle(size_t& indexX, size_t& indexY, size_t& sizeX, size_t& sizeY)
{
	std::vector<size_t> leftBuffer(DungeonMap::Height, 0), rightBuffer(DungeonMap::Height, DungeonMap::Height), topBuffer(DungeonMap::Height, 0), heightBuffer(DungeonMap::Height, 0);
	size_t maximalArea = 0, area = 0;

	for (size_t row = 0; row < DungeonMap::Width; ++row)
	{
		size_t curLeft = 0, curRight = DungeonMap::Height;

		for (size_t column = 0; column < DungeonMap::Height; ++column)
		{
			if (DungeonMap::Map[row][column] == DungeonTile::Floor || DungeonMap::Map[row][column] == DungeonTile::Wall)
			{
				heightBuffer[column]++;
				topBuffer[column] = row;
			}

			else
				heightBuffer[column] = 0;
		}

		for (size_t column = 0; column < DungeonMap::Height; ++column)
		{
			if (DungeonMap::Map[row][column] == DungeonTile::Floor || DungeonMap::Map[row][column] == DungeonTile::Wall)
				leftBuffer[column] = std::max(leftBuffer[column], curLeft);

			else
			{
				leftBuffer[column] = 0;
				curLeft = column + 1;
			}
		}

		for (int64_t column = static_cast<int64_t>(DungeonMap::Height) - 1; column >= 0; --column)
		{
			if (DungeonMap::Map[row][column] == DungeonTile::Floor || DungeonMap::Map[row][column] == DungeonTile::Wall)
				rightBuffer[column] = std::min(rightBuffer[column], curRight);

			else
			{
				rightBuffer[column] = DungeonMap::Height;
				curRight = column;
			}
		}

		for (size_t column = 0; column < DungeonMap::Height; ++column)
		{
			area = (rightBuffer[column] - leftBuffer[column]) * heightBuffer[column];

			if (area > maximalArea)
			{
				maximalArea = area;
				indexX = leftBuffer[column];

				if (topBuffer[column] >= heightBuffer[column])
					indexY = topBuffer[column] - heightBuffer[column] + 1;

				else
					indexY = 0;

				sizeX = rightBuffer[column] - leftBuffer[column];
				sizeY = heightBuffer[column];
			}
		}
	}

	return maximalArea;
}

const bool DungeonMap::EraseMaxRectangle(const size_t indexX, const size_t indexY, const size_t sizeX, const size_t sizeY)
{
	for (size_t row = indexY; row < indexY + sizeY; ++row)
		for (size_t column = indexX; column < indexX + sizeX; ++column)
			DungeonMap::Map[row][column] = DungeonTile::Unused;

	return true;
}

const bool DungeonMap::GenerateFeatures(const size_t features)
{
	if (features == 0)
		return GenerateFeatures((1ULL << 24));

	else if (!DungeonMap::PlaceRoom(DungeonMap::Width >> 1, DungeonMap::Height >> 1, static_cast<uint8_t>(DungeonMap::RandomUnsigned(0, 3), true)))
		return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t index = 1; index < features; ++index)
		if (!DungeonMap::PlaceFeature())
			break;

	if (!DungeonMap::PlaceObject(DungeonTile::AscendDoor))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonMap::PlaceObject(DungeonTile::DescendDoor))
		return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t index = 0; index < DungeonMap::Tiles.size(); ++index)
	{
		if (DungeonMap::Tiles[index] == DungeonTile::Unused)
			DungeonMap::Tiles[index] = DungeonTile::Floor;

		else if (DungeonMap::Tiles[index] == DungeonTile::Floor || DungeonMap::Tiles[index] == DungeonTile::Corridor)
			DungeonMap::Tiles[index] = DungeonTile::Unused;
	}

	return true;
}

const bool DungeonMap::GenerateMap(const size_t sizeX, const size_t sizeY, size_t& tileCount)
{
	const float scaleFactor = 1.0f;// 0.0625f;
	size_t offset = 0, featureCount = static_cast<size_t>(glm::sqrt(static_cast<float>(sizeX * sizeY) * scaleFactor));

	DungeonMap::Tiles.resize(sizeX * sizeY, DungeonTile::Unused);
	DungeonMap::Width = static_cast<int32_t>(sizeX);
	DungeonMap::Height = static_cast<int32_t>(sizeY);

	for (size_t index = 0; index < 8; ++index)
	{
		if (DungeonMap::GenerateFeatures(featureCount))
			break;

		else if (index + 1 == 8)
			return Logger::LogError(__FUNCTION__, __LINE__);
	}

	DungeonMap::Map = std::vector<std::vector<uint8_t>>(sizeX, std::vector<uint8_t>(sizeY, DungeonTile::Floor));
	tileCount = 0;

	for (size_t indexX = 0; indexX < sizeX; ++indexX)
		for (size_t indexY = 0; indexY < sizeY; ++indexY)
		{
			DungeonMap::Map[indexX][indexY] = DungeonMap::Tiles[offset];
			++offset;

			if (DungeonMap::Map[indexX][indexY] == DungeonTile::Floor || DungeonMap::Map[indexX][indexY] == DungeonTile::Wall)
				++tileCount;
		}

	DungeonMap::Rooms.clear();
	DungeonMap::Exits.clear();
	DungeonMap::Tiles.clear();

	return true;
}

const uint8_t DungeonMap::GetTile(const int32_t x, const int32_t y)
{
	if (x < 0 || y < 0 || x >= DungeonMap::Width || y >= DungeonMap::Height)
		return DungeonTile::Unused;

	return DungeonMap::Tiles[static_cast<size_t>(x) + static_cast<size_t>(y) * static_cast<size_t>(DungeonMap::Width)];
}

const bool DungeonMap::IsRoomCorner(const size_t x, const size_t y)
{
	bool z[8] = { false, false, false, false, false, false, false, false };

	if (DungeonMap::Map[x][y] == DungeonTile::Unused)
	{
		z[0] = DungeonMap::Map[x - 1][y - 1] == DungeonTile::Wall;
		z[1] = DungeonMap::Map[x][y - 1] == DungeonTile::Wall;
		z[2] = DungeonMap::Map[x + 1][y - 1] == DungeonTile::Wall;
		z[3] = DungeonMap::Map[x + 1][y] == DungeonTile::Wall;
		z[4] = DungeonMap::Map[x + 1][y + 1] == DungeonTile::Wall;
		z[5] = DungeonMap::Map[x][y + 1] == DungeonTile::Wall;
		z[6] = DungeonMap::Map[x - 1][y + 1] == DungeonTile::Wall;
		z[7] = DungeonMap::Map[x - 1][y] == DungeonTile::Wall;

		if ((z[7] && z[0] && z[1] && !z[3] && !z[4] && !z[5]) || (z[1] && z[2] && z[3] && !z[5] && !z[6] && !z[7]) || (z[3] && z[4] && z[5] && !z[7] && !z[0] && !z[1]) || (z[5] && z[6] && z[7] && !z[1] && !z[2] && !z[3]))
			return true;
	}

	return false;
}

const bool DungeonMap::IsCorridorEnd(const size_t x, const size_t y)
{
	bool z[8] = { false, false, false, false, false, false, false, false };

	if (DungeonMap::Map[x][y] == DungeonTile::Unused)
	{
		z[0] = DungeonMap::Map[x - 1][y - 1] == DungeonTile::Wall;
		z[1] = DungeonMap::Map[x][y - 1] == DungeonTile::Wall;
		z[2] = DungeonMap::Map[x + 1][y - 1] == DungeonTile::Wall;
		z[3] = DungeonMap::Map[x + 1][y] == DungeonTile::Wall;
		z[4] = DungeonMap::Map[x + 1][y + 1] == DungeonTile::Wall;
		z[5] = DungeonMap::Map[x][y + 1] == DungeonTile::Wall;
		z[6] = DungeonMap::Map[x - 1][y + 1] == DungeonTile::Wall;
		z[7] = DungeonMap::Map[x - 1][y] == DungeonTile::Wall;

		if ((z[7] && z[0] && z[1] && z[2] && z[3]) || (z[1] && z[2] && z[3] && z[4] && z[5]) || (z[3] && z[4] && z[5] && z[6] && z[7]) || (z[5] && z[6] && z[7] && z[0] && z[1]))
			return true;
	}

	return false;
}

const bool DungeonMap::IsIsolated(const size_t x, const size_t y)
{
	bool z[8] = { false, false, false, false, false, false, false, false };

	if (DungeonMap::Map[x][y] == DungeonTile::Unused)
	{
		z[0] = DungeonMap::Map[x - 1][y - 1] == DungeonTile::Unused;
		z[1] = DungeonMap::Map[x][y - 1] == DungeonTile::Unused;
		z[2] = DungeonMap::Map[x + 1][y - 1] == DungeonTile::Unused;
		z[3] = DungeonMap::Map[x + 1][y] == DungeonTile::Unused;
		z[4] = DungeonMap::Map[x + 1][y + 1] == DungeonTile::Unused;
		z[5] = DungeonMap::Map[x][y + 1] == DungeonTile::Unused;
		z[6] = DungeonMap::Map[x - 1][y + 1] == DungeonTile::Unused;
		z[7] = DungeonMap::Map[x - 1][y] == DungeonTile::Unused;

		if (z[0] && z[1] && z[2] && z[3] && z[4] && z[5] && z[6] && z[7])
			return true;
	}

	return false;
}

const bool DungeonMap::PlaceCorridor(const int32_t x, const int32_t y, const uint8_t direction)
{
	Rectangle corridor(x, y, 0, 0);

	if (DungeonMap::RandomBoolean())
	{
		corridor.Width = DungeonMap::RandomSigned(MIN_CORRIDOR_LENGTH, MAX_CORRIDOR_LENGTH);
		corridor.Height = 1;

		if (direction == 0)
		{
			corridor.Y = y - 1;

			if (DungeonMap::RandomBoolean())
				corridor.X = x - corridor.Width + 1;
		}

		else if (direction == 1)
		{
			corridor.Y = y + 1;

			if (DungeonMap::RandomBoolean())
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
		corridor.Height = DungeonMap::RandomSigned(MIN_CORRIDOR_LENGTH, MAX_CORRIDOR_LENGTH);

		if (direction == 0)
			corridor.Y = y - corridor.Height;

		else if (direction == 1)
			corridor.Y = y + 1;

		else if (direction == 2)
		{
			corridor.X = x - 1;

			if (DungeonMap::RandomBoolean())
				corridor.Y = y - corridor.Height + 1;
		}

		else if (direction == 3)
		{
			corridor.X = x + 1;

			if (DungeonMap::RandomBoolean())
				corridor.Y = y - corridor.Height + 1;
		}
	}

	if (DungeonMap::PlaceRectangle(corridor, DungeonTile::Corridor))
	{
		if (direction != 0 && corridor.Width != 1)
			DungeonMap::Exits.emplace_back(Rectangle(corridor.X, corridor.Y + corridor.Height, corridor.Width, 1));

		if (direction != 1 && corridor.Width != 1)
			DungeonMap::Exits.emplace_back(Rectangle(corridor.X, corridor.Y - 1, corridor.Width, 1));

		if (direction != 2 && corridor.Height != 1)
			DungeonMap::Exits.emplace_back(Rectangle(corridor.X + corridor.Width, corridor.Y, 1, corridor.Height));

		if (direction != 3 && corridor.Height != 1)
			DungeonMap::Exits.emplace_back(Rectangle(corridor.X - 1, corridor.Y, 1, corridor.Height));

		return true;
	}

	return false;
}

const bool DungeonMap::PlaceFeature()
{
	size_t randomIndex = 0, randomOffset = 0;
	int32_t x = 0, y = 0;
	uint8_t directions[4] = { 0, 1, 2, 3 };

	for (size_t index = 0; index < 64; ++index)
	{
		randomIndex = DungeonMap::RandomUnsigned(0, 3);
		randomOffset = DungeonMap::RandomUnsigned(0, 3);

		if (randomIndex != randomOffset)
			std::swap(directions[randomIndex], directions[randomOffset]);
	}

	for (size_t index = 0; index < (1ULL << 16); ++index)
	{
		randomIndex = DungeonMap::RandomUnsigned(0, DungeonMap::Exits.size() - 1);
		x = DungeonMap::RandomSigned(DungeonMap::Exits[randomIndex].X, DungeonMap::Exits[randomIndex].X + DungeonMap::Exits[randomIndex].Width - 1);
		y = DungeonMap::RandomSigned(DungeonMap::Exits[randomIndex].Y, DungeonMap::Exits[randomIndex].Y + DungeonMap::Exits[randomIndex].Height - 1);

		for (uint8_t direction = 0; direction < 4; ++direction)
			if (DungeonMap::PlaceFeature(x, y, directions[direction]))
			{
				DungeonMap::Exits.erase(DungeonMap::Exits.begin() + randomIndex);
				return true;
			}
	}

	return false;
}

const bool DungeonMap::PlaceFeature(const int32_t x, const int32_t y, const uint8_t direction)
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

	if (DungeonMap::GetTile(x + dx, y + dy) != DungeonTile::Floor && DungeonMap::GetTile(x + dx, y + dy) != DungeonTile::Corridor)
		return false;

	else if (DungeonMap::RandomSigned(1, 100) < ROOM_PROBABILITY && DungeonMap::PlaceRoom(x, y, direction))
	{
		DungeonMap::SetTile(x, y, DungeonTile::ClosedDoor);
		return true;
	}

	else if (DungeonMap::PlaceCorridor(x, y, direction))
	{
		if (DungeonMap::GetTile(x + dx, y + dy) == DungeonTile::Floor)
			DungeonMap::SetTile(x, y, DungeonTile::ClosedDoor);

		else
			DungeonMap::SetTile(x, y, DungeonTile::Corridor);

		return true;
	}

	return false;
}

const bool DungeonMap::PlaceObject(const uint8_t tile)
{
	size_t randomIndex = 0;
	int32_t x = 0, y = 0;

	if (DungeonMap::Rooms.empty())
		return false;

	randomIndex = DungeonMap::RandomUnsigned(0, DungeonMap::Rooms.size() - 1);
	x = DungeonMap::RandomSigned(DungeonMap::Rooms[randomIndex].X + 1, DungeonMap::Rooms[randomIndex].X + DungeonMap::Rooms[randomIndex].Width - 2);
	y = DungeonMap::RandomSigned(DungeonMap::Rooms[randomIndex].Y + 1, DungeonMap::Rooms[randomIndex].Y + DungeonMap::Rooms[randomIndex].Height - 2);

	if (DungeonMap::GetTile(x, y) == DungeonTile::Floor)
	{
		DungeonMap::SetTile(x, y, tile);
		DungeonMap::Rooms.erase(DungeonMap::Rooms.begin() + randomIndex);

		return true;
	}

	return false;
}

const bool DungeonMap::PlaceObjects()
{
	const float lootScalar = 0.25f;//0.125f;
	std::vector<std::pair<size_t, size_t>> lootChoices, lootPoints;
	std::vector<float> distances;
	std::vector<uint8_t> imageData;
	size_t index = 0, lootCount = static_cast<size_t>(glm::sqrt(static_cast<float>(DungeonMap::Width) * static_cast<float>(DungeonMap::Height)) * lootScalar), treasureCount = lootCount >> 2UL, lootIndex = 0, maxIndex = 0;
	float maxDistance = 0.0f;

	for (size_t indexX = 1; indexX < static_cast<size_t>(DungeonMap::Width - 1); ++indexX)
		for (size_t indexY = 1; indexY < static_cast<size_t>(DungeonMap::Height - 1); ++indexY)
		{
			lootIndex = DungeonMap::RandomUnsigned(0, 99);

			if (lootIndex >= 25 && DungeonMap::IsRoomCorner(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));

			else if (lootIndex < 25 && DungeonMap::IsCorridorEnd(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));
		}

	if (lootChoices.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	lootIndex = DungeonMap::RandomUnsigned(0, lootChoices.size() - 1);
	lootPoints.push_back(lootChoices[lootIndex]);
	lootChoices.erase(lootChoices.begin() + lootIndex);

	for (lootIndex = 1; lootIndex < lootCount; ++lootIndex)
	{
		distances = std::vector<float>(lootChoices.size(), std::numeric_limits<float>::max());
		maxIndex = 0;
		maxDistance = -0.0f;

		for (size_t choiceIndex = 0; choiceIndex < lootChoices.size(); ++choiceIndex)
			for (size_t pointIndex = 0; pointIndex < lootPoints.size(); ++pointIndex)
				if (distances[choiceIndex] > Distance(lootChoices[choiceIndex], lootPoints[pointIndex]))
					distances[choiceIndex] = Distance(lootChoices[choiceIndex], lootPoints[pointIndex]);

		for (size_t index = 0; index < distances.size(); ++index)
			if (distances[index] > maxDistance)
			{
				maxIndex = index;
				maxDistance = distances[index];
			}

		if (lootChoices.empty())
			break;

		lootPoints.push_back(lootChoices[maxIndex]);
		lootChoices.erase(lootChoices.begin() + maxIndex);
	}

	for (size_t index = 0; index < lootPoints.size(); ++index)
		DungeonMap::Map[lootPoints[index].first][lootPoints[index].second] = DungeonTile::Barrel;

	lootChoices.clear();
	lootPoints.clear();

	for (size_t indexX = 1; indexX < static_cast<size_t>(DungeonMap::Width - 1); ++indexX)
		for (size_t indexY = 1; indexY < static_cast<size_t>(DungeonMap::Height - 1); ++indexY)
			if (DungeonMap::IsCorridorEnd(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));

	if (lootChoices.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	lootIndex = DungeonMap::RandomUnsigned(0, lootChoices.size() - 1);
	lootPoints.push_back(lootChoices[lootIndex]);
	lootChoices.erase(lootChoices.begin() + lootIndex);

	for (lootIndex = 1; lootIndex < treasureCount; ++lootIndex)
	{
		distances = std::vector<float>(lootChoices.size(), std::numeric_limits<float>::max());
		maxIndex = 0;
		maxDistance = -0.0f;

		for (size_t choiceIndex = 0; choiceIndex < lootChoices.size(); ++choiceIndex)
			for (size_t pointIndex = 0; pointIndex < lootPoints.size(); ++pointIndex)
				if (distances[choiceIndex] > Distance(lootChoices[choiceIndex], lootPoints[pointIndex]))
					distances[choiceIndex] = Distance(lootChoices[choiceIndex], lootPoints[pointIndex]);

		for (size_t index = 0; index < distances.size(); ++index)
			if (distances[index] > maxDistance)
			{
				maxIndex = index;
				maxDistance = distances[index];
			}

		if (lootChoices.empty())
			break;

		lootPoints.push_back(lootChoices[maxIndex]);
		lootChoices.erase(lootChoices.begin() + maxIndex);
	}

	for (size_t index = 0; index < lootPoints.size(); ++index)
		DungeonMap::Map[lootPoints[index].first][lootPoints[index].second] = DungeonTile::Chest;

	std::pair<size_t, size_t> startingPoint;

	for (size_t indexX = 0; indexX < DungeonMap::Width; ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Height; ++indexY)
			if (DungeonMap::Map[indexX][indexY] == DungeonTile::AscendDoor)
			{
				startingPoint = std::make_pair(indexX, indexY);
				indexX = DungeonMap::Width;

				break;
			}

	lootPoints.clear();
	lootChoices.clear();

	for (size_t indexX = 1; indexX < static_cast<size_t>(DungeonMap::Width - 1); ++indexX)
		for (size_t indexY = 1; indexY < static_cast<size_t>(DungeonMap::Height - 1); ++indexY)
			if (DungeonMap::IsIsolated(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));

	if (lootChoices.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	distances = std::vector<float>(lootChoices.size(), -0.0f);
	maxIndex = 0;
	maxDistance = -0.0f;

	for (size_t index = 0; index < lootChoices.size(); ++index)
		if (Distance(startingPoint, lootChoices[index]) > maxDistance)
		{
			maxDistance = Distance(startingPoint, lootChoices[index]);
			maxIndex = index;
		}

	lootPoints.emplace_back(lootChoices[maxIndex]);

	for (size_t index = 0; index < lootPoints.size(); ++index)
		DungeonMap::Map[lootPoints[index].first][lootPoints[index].second] = DungeonTile::Enemy;

	return true;
}

const bool DungeonMap::PlaceRectangle(const Rectangle& rectangle, const uint8_t tile)
{
	if (rectangle.X < 1 || rectangle.Y < 1 || rectangle.X + rectangle.Width > DungeonMap::Width - 1 || rectangle.Y + rectangle.Height > DungeonMap::Height - 1)
		return false;

	for (int32_t indexX = rectangle.X; indexX < rectangle.X + rectangle.Width; ++indexX)
		for (int32_t indexY = rectangle.Y; indexY < rectangle.Y + rectangle.Height; ++indexY)
			if (DungeonMap::GetTile(indexX, indexY) != DungeonTile::Unused)
				return false;

	for (int32_t indexX = rectangle.X - 1; indexX < rectangle.X + rectangle.Width + 1; ++indexX)
		for (int32_t indexY = rectangle.Y - 1; indexY < rectangle.Y + rectangle.Height + 1; ++indexY)
		{
			if (indexX == rectangle.X - 1 || indexY == rectangle.Y - 1 || indexX == rectangle.X + rectangle.Width || indexY == rectangle.Y + rectangle.Height)
				DungeonMap::SetTile(indexX, indexY, DungeonTile::Wall);

			else
				DungeonMap::SetTile(indexX, indexY, tile);
		}

	return true;
}

const bool DungeonMap::PlaceRoom(const int32_t x, const int32_t y, const uint8_t direction, const bool firstRoom)
{
	Rectangle room(0, 0, DungeonMap::RandomSigned(MIN_ROOM_SIZE, MAX_ROOM_SIZE), DungeonMap::RandomSigned(MIN_ROOM_SIZE, MAX_ROOM_SIZE));

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

	if (!DungeonMap::PlaceRectangle(room, DungeonTile::Floor))
		return false;

	else
	{
		DungeonMap::Rooms.emplace_back(room);

		if (direction != 0 || firstRoom)
			DungeonMap::Exits.emplace_back(Rectangle(room.X, room.Y + room.Height, room.Width, 1));

		if (direction != 1 || firstRoom)
			DungeonMap::Exits.emplace_back(Rectangle(room.X, room.Y - 1, room.Width, 1));

		if (direction != 2 || firstRoom)
			DungeonMap::Exits.emplace_back(Rectangle(room.X + room.Width, room.Y, 1, room.Height));

		if (direction != 3 || firstRoom)
			DungeonMap::Exits.emplace_back(Rectangle(room.X - 1, room.Y, 1, room.Height));
	}

	return true;
}

const bool DungeonMap::SetTile(const int32_t x, const int32_t y, const uint8_t tile)
{
	if (x < 0 || y < 0 || x >= DungeonMap::Width || y >= DungeonMap::Height)
		return Logger::LogError(__FUNCTION__, __LINE__);

	DungeonMap::Tiles[static_cast<size_t>(x) + static_cast<size_t>(y) * static_cast<size_t>(DungeonMap::Width)] = static_cast<DungeonTile>(tile);
	return true;
}