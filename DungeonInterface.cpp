#include "Assets.hpp"
#include "DungeonInterface.hpp"
#include "DungeonScene.hpp"
#include "Input.hpp"
#include "Logger.hpp"

#include <glfw/glfw3.h>
#include <limits>

std::chrono::time_point<std::chrono::steady_clock> DungeonInterface::Frame;
size_t DungeonInterface::DeltaFrame = std::numeric_limits<size_t>::max(), DungeonInterface::Index = 0;
InterfaceType DungeonInterface::Type = InterfaceType::NULL_INTERFACE;

const bool DungeonInterface::Active()
{
	return DungeonInterface::Type != InterfaceType::NULL_INTERFACE;
}

const bool DungeonInterface::Open(const InterfaceType& type)
{
	if (DungeonInterface::Active())
		return Logger::LogError(__FUNCTION__, __LINE__);

	DungeonInterface::Type = type;
	return true;
}

const bool DungeonInterface::Open(const InterfaceType& type, const size_t index)
{
	if (DungeonInterface::Active())
		return Logger::LogError(__FUNCTION__, __LINE__);

	DungeonInterface::Index = index;
	DungeonInterface::Type = type;

	return true;
}

const bool DungeonInterface::Render()
{
	glm::vec2 customScale, maxScale, cursorPosition, cellPosition, nextPosition;
	std::string imageName, itemName;
	size_t boxSize = 0, index = 0;

	if (DungeonInterface::Type == InterfaceType::GAME_USER_INVENTORY || DungeonInterface::Type == InterfaceType::GAME_CONTAINER_INVENTORY)
	{
		if (DungeonInterface::Type == InterfaceType::GAME_USER_INVENTORY)
		{
			customScale = glm::vec2(512.0f);
			maxScale = glm::vec2(1024.0f);
			imageName = "Inventory";
			boxSize = 4;
			DungeonInterface::Index = 0;
		}

		else if (DungeonInterface::Type == InterfaceType::GAME_CONTAINER_INVENTORY)
		{
			if (DungeonScene::Containers[DungeonInterface::Index].Object.Type == ObjectType::WOODEN_BARREL)
			{
				DungeonScene::Containers[DungeonInterface::Index].CreateCommon();

				customScale = glm::vec2(256.0f);
				maxScale = glm::vec2(1024.0f);
				imageName = "Inventory_Common";
				boxSize = 2;
			}

			else if (DungeonScene::Containers[DungeonInterface::Index].Object.Type == ObjectType::WOODEN_CHEST)
			{
				DungeonScene::Containers[DungeonInterface::Index].CreateTreasure();

				customScale = glm::vec2(384.0f);
				maxScale = glm::vec2(1024.0f);
				imageName = "Inventory_Rare";
				boxSize = 3;
			}
		}

		if (!Assets::Images[imageName].Position((maxScale - customScale) * 0.5f, maxScale, true))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!Assets::Images[imageName].Scale(customScale, maxScale))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!Assets::Images[imageName].Render())
			return Logger::LogError(__FUNCTION__, __LINE__);

		cursorPosition = glm::vec2(Input::CursorNowX(), Input::CursorNowY());

		for (size_t indexX = 0; indexX < boxSize; ++indexX)
			for (size_t indexY = 0; indexY < boxSize; ++indexY)
			{
				itemName = DungeonItem::ItemName(DungeonScene::Containers[DungeonInterface::Index].Items[index].Type);
				cellPosition = Assets::Images[imageName].Model * glm::vec4(glm::vec2(static_cast<float>(indexX), static_cast<float>(indexY)) / static_cast<float>(boxSize), 0.0f, 1.0f);
				nextPosition = Assets::Images[imageName].Model * glm::vec4(glm::vec2(static_cast<float>(indexX + 1), static_cast<float>(indexY + 1)) / static_cast<float>(boxSize), 0.0f, 1.0f);

				if (!itemName.empty())
				{
					if (!Assets::Images[itemName].Position(glm::vec2(cellPosition.y, cellPosition.x), true))
						return Logger::LogError(__FUNCTION__, __LINE__);

					else if (!Assets::Images[itemName].Scale(glm::vec2(128.0f), glm::vec2(1024.0f)))
						return Logger::LogError(__FUNCTION__, __LINE__);

					else if (!Assets::Images[itemName].Render())
						return Logger::LogError(__FUNCTION__, __LINE__);
				}

				if (cursorPosition.x >= cellPosition.x && cursorPosition.x <= nextPosition.x && cursorPosition.y >= cellPosition.y && cursorPosition.y <= nextPosition.y)
				{
					if (!Assets::Images["Inventory_Selected"].Position(cellPosition, true))
						return Logger::LogError(__FUNCTION__, __LINE__);

					else if (!Assets::Images["Inventory_Selected"].Scale(glm::vec2(128.0f), glm::vec2(1024.0f)))
						return Logger::LogError(__FUNCTION__, __LINE__);

					else if (!Assets::Images["Inventory_Selected"].Render())
						return Logger::LogError(__FUNCTION__, __LINE__);
				}

				++index;
			}
	}

	return true;
}

const bool DungeonInterface::Update()
{
	const size_t menuWaitTime = 333;

	DungeonInterface::DeltaFrame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - DungeonInterface::Frame).count();

	if (DungeonInterface::DeltaFrame >= menuWaitTime)
	{
		if (DungeonInterface::Type == InterfaceType::NULL_INTERFACE)
		{
			if (Input::KeyNowPressed(GLFW_KEY_TAB))
			{
				if (!DungeonInterface::Open(InterfaceType::GAME_USER_INVENTORY))
					return Logger::LogError(__FUNCTION__, __LINE__);

				DungeonInterface::Frame = std::chrono::high_resolution_clock::now();
			}
		}

		else
		{
			if (DungeonInterface::Type == InterfaceType::GAME_USER_INVENTORY)
			{
				if (Input::KeyNowPressed(GLFW_KEY_TAB) || Input::KeyNowPressed(GLFW_KEY_ESCAPE))
				{
					DungeonInterface::Frame = std::chrono::high_resolution_clock::now();
					DungeonInterface::Type = InterfaceType::NULL_INTERFACE;
				}
			}

			if (DungeonInterface::Type == InterfaceType::GAME_CONTAINER_INVENTORY)
			{
				if (Input::KeyNowPressed(GLFW_KEY_TAB) || Input::KeyNowPressed(GLFW_KEY_ESCAPE))
				{
					DungeonInterface::Frame = std::chrono::high_resolution_clock::now();
					DungeonInterface::Type = InterfaceType::NULL_INTERFACE;
				}
			}
		}
	}

	return true;
}