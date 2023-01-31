#include "DungeonData.hpp"
#include "DungeonDebugger.hpp"
#include "DungeonMap.hpp"
#include "MathManager.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT

#include "Libraries/stb/stb_image_write.h"

bool DungeonDebugger::IsRoomCorner(const size_t x, const size_t y)
{
	bool z[8] = { false };

	if (DungeonMap::Map[x][y] == DungeonObject::Unused)
	{
		z[0] = DungeonMap::Map[x - 1][y - 1] == DungeonObject::Wall;
		z[1] = DungeonMap::Map[x][y - 1] == DungeonObject::Wall;
		z[2] = DungeonMap::Map[x + 1][y - 1] == DungeonObject::Wall;
		z[3] = DungeonMap::Map[x + 1][y] == DungeonObject::Wall;
		z[4] = DungeonMap::Map[x + 1][y + 1] == DungeonObject::Wall;
		z[5] = DungeonMap::Map[x][y + 1] == DungeonObject::Wall;
		z[6] = DungeonMap::Map[x - 1][y + 1] == DungeonObject::Wall;
		z[7] = DungeonMap::Map[x - 1][y] == DungeonObject::Wall;

		if ((z[7] && z[0] && z[1] && !z[3] && !z[4] && !z[5]) || (z[1] && z[2] && z[3] && !z[5] && !z[6] && !z[7]) || (z[3] && z[4] && z[5] && !z[7] && !z[0] && !z[1]) || (z[5] && z[6] && z[7] && !z[1] && !z[2] && !z[3]))
			return true;
	}

	return false;
}

bool DungeonDebugger::IsCorridorEnd(const size_t x, const size_t y)
{
	bool z[8] = { false };

	if (DungeonMap::Map[x][y] == DungeonObject::Unused)
	{
		z[0] = DungeonMap::Map[x - 1][y - 1] == DungeonObject::Wall;
		z[1] = DungeonMap::Map[x][y - 1] == DungeonObject::Wall;
		z[2] = DungeonMap::Map[x + 1][y - 1] == DungeonObject::Wall;
		z[3] = DungeonMap::Map[x + 1][y] == DungeonObject::Wall;
		z[4] = DungeonMap::Map[x + 1][y + 1] == DungeonObject::Wall;
		z[5] = DungeonMap::Map[x][y + 1] == DungeonObject::Wall;
		z[6] = DungeonMap::Map[x - 1][y + 1] == DungeonObject::Wall;
		z[7] = DungeonMap::Map[x - 1][y] == DungeonObject::Wall;

		if ((z[7] && z[0] && z[1] && z[2] && z[3]) || (z[1] && z[2] && z[3] && z[4] && z[5]) || (z[3] && z[4] && z[5] && z[6] && z[7]) || (z[5] && z[6] && z[7] && z[0] && z[1]))
			return true;
	}

	return false;
}

bool DungeonDebugger::IsIsolated(const size_t x, const size_t y)
{
	bool z[8] = { false };

	if (DungeonMap::Map[x][y] == DungeonObject::Unused)
	{
		z[0] = DungeonMap::Map[x - 1][y - 1] == DungeonObject::Unused;
		z[1] = DungeonMap::Map[x][y - 1] == DungeonObject::Unused;
		z[2] = DungeonMap::Map[x + 1][y - 1] == DungeonObject::Unused;
		z[3] = DungeonMap::Map[x + 1][y] == DungeonObject::Unused;
		z[4] = DungeonMap::Map[x + 1][y + 1] == DungeonObject::Unused;
		z[5] = DungeonMap::Map[x][y + 1] == DungeonObject::Unused;
		z[6] = DungeonMap::Map[x - 1][y + 1] == DungeonObject::Unused;
		z[7] = DungeonMap::Map[x - 1][y] == DungeonObject::Unused;

		if (z[0] && z[1] && z[2] && z[3] && z[4] && z[5] && z[6] && z[7])
			return true;
	}

	return false;
}

static float Distance(const std::pair<size_t, size_t>& x, const std::pair<size_t, size_t>& y)
{
	const float dx = static_cast<float>(x.first) - static_cast<float>(y.first), dy = static_cast<float>(x.second) - static_cast<float>(y.second);
	return std::sqrtf(dx * dx + dy * dy);
}

bool DungeonDebugger::WriteImage(const std::string& imageFile)
{
	std::vector<std::vector<uint8_t>> imageBuffer;
	std::vector<std::pair<size_t, size_t>> lootChoices, lootPoints;
	std::vector<float> distances;
	std::vector<uint8_t> imageData;
	size_t index = 0, lootCount = static_cast<size_t>(sqrtf(static_cast<float>(DungeonMap::Map.size() * DungeonMap::Map[0].size())) * 0.125f), treasureCount = lootCount >> 2, lootIndex = 0, maxIndex = 0;
	float maxDistance = 0.0f;

	imageBuffer = std::vector<std::vector<uint8_t>>(DungeonMap::Map.size(), std::vector<uint8_t>(DungeonMap::Map[0].size() * 3, 0));

	for (size_t indexX = 0; indexX < DungeonMap::Map.size(); ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Map[0].size(); ++indexY)
		{
			lootIndex = MathManager::RandomUnsigned(0, 99);

			if (lootIndex >= 25 && IsRoomCorner(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));

			else if (lootIndex < 25 && IsCorridorEnd(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));
		}

	lootIndex = MathManager::RandomUnsigned(0, lootChoices.size() - 1);
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
		DungeonMap::Map[lootPoints[index].first][lootPoints[index].second] = DungeonObject::Barrel;

	lootChoices.clear();
	lootPoints.clear();

	for (size_t indexX = 0; indexX < DungeonMap::Map.size(); ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Map[0].size(); ++indexY)
			if (IsCorridorEnd(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));

	lootIndex = MathManager::RandomUnsigned(0, lootChoices.size() - 1);
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
		DungeonMap::Map[lootPoints[index].first][lootPoints[index].second] = DungeonObject::Chest;

	//
	std::pair<size_t, size_t> startingPoint;

	for (size_t indexX = 0; indexX < DungeonMap::Map.size(); ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Map[0].size(); ++indexY)
			if (DungeonMap::Map[indexX][indexY] == DungeonObject::AscendDoor)
				startingPoint = std::make_pair(indexX, indexY);

	lootPoints.clear();
	lootChoices.clear();

	for (size_t indexX = 0; indexX < DungeonMap::Map.size(); ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Map[0].size(); ++indexY)
			if (IsIsolated(indexX, indexY))
				lootChoices.push_back(std::make_pair(indexX, indexY));

	distances = std::vector<float>(lootChoices.size(), -0.0f);
	maxIndex = 0;
	maxDistance = 1000.0f;

	for (size_t index = 0; index < lootChoices.size(); ++index)
		if (Distance(startingPoint, lootChoices[index]) < maxDistance)
		{
			maxDistance = Distance(startingPoint, lootChoices[index]);
			maxIndex = index;
		}

	lootPoints.emplace_back(lootChoices[maxIndex]);

	for (size_t index = 0; index < lootPoints.size(); ++index)
		DungeonMap::Map[lootPoints[index].first][lootPoints[index].second] = DungeonObject::Enemy;
	//

	for (size_t indexX = 0; indexX < imageBuffer.size(); ++indexX)
		for (size_t indexY = 0; indexY < imageBuffer[0].size() / 3; ++indexY)
		{
			if (DungeonMap::Map[indexX][indexY] == DungeonObject::Unused || DungeonMap::Map[indexX][indexY] == DungeonObject::Corridor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 90;
				imageBuffer[indexX][indexY * 3 + 1] = 60;
				imageBuffer[indexX][indexY * 3 + 2] = 40;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::Wall)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 160;
				imageBuffer[indexX][indexY * 3 + 1] = 160;
				imageBuffer[indexX][indexY * 3 + 2] = 160;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::AscendDoor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 104;
				imageBuffer[indexX][indexY * 3 + 2] = 104;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::DescendDoor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 0;
				imageBuffer[indexX][indexY * 3 + 2] = 0;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::ClosedDoor)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 76;
				imageBuffer[indexX][indexY * 3 + 1] = 255;
				imageBuffer[indexX][indexY * 3 + 2] = 0;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::Barrel)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 255;
				imageBuffer[indexX][indexY * 3 + 2] = 255;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::Chest)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 255;
				imageBuffer[indexX][indexY * 3 + 1] = 255;
				imageBuffer[indexX][indexY * 3 + 2] = 0;
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::Enemy)
			{
				imageBuffer[indexX][indexY * 3 + 0] = 0;
				imageBuffer[indexX][indexY * 3 + 1] = 0;
				imageBuffer[indexX][indexY * 3 + 2] = 255;
			}
		}

	for (size_t indexX = 0; indexX < (imageBuffer.size() >> 1); ++indexX)
		for (size_t indexY = 0; indexY < imageBuffer[0].size(); ++indexY)
			std::swap(imageBuffer[indexX][indexY], imageBuffer[imageBuffer.size() - 1 - indexX][indexY]);

	if (!imageFile.empty())
	{
		index = 0;
		imageData = std::vector<uint8_t>(imageBuffer.size() * imageBuffer[0].size());

		for (size_t indexX = 0; indexX < imageBuffer.size(); ++indexX)
			for (size_t indexY = 0; indexY < imageBuffer[0].size(); ++indexY)
			{
				imageData[index] = imageBuffer[indexX][indexY];
				++index;
			}

		stbi_write_png(imageFile.c_str(), static_cast<int>(DungeonMap::Map.size()), static_cast<int>(DungeonMap::Map[0].size()), 3, &imageData[0], static_cast<int>(DungeonMap::Map.size()) * 3);
	}

	return true;
}