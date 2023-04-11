#include "Assets.hpp"
#include "Camera.hpp"
#include "DungeonAssets.hpp"
#include "DungeonItem.hpp"
#include "DungeonInterface.hpp"
#include "DungeonMap.hpp"
#include "DungeonModel.hpp"
#include "DungeonScene.hpp"
#include "Input.hpp"
#include "Logger.hpp"

#include <glfw/glfw3.h>

std::vector<DungeonObject> DungeonScene::Objects;
std::vector<DungeonContainer> DungeonScene::Containers;
std::vector<Light> DungeonScene::Lights;

static const size_t LevelSize(const size_t level) { return (level << 3) + 32; }

const bool DungeonScene::Destroy()
{
	DungeonScene::Objects.clear();
	DungeonScene::Lights.clear();

	if (!Assets::Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonMap::Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool DungeonScene::Generate(const size_t level)
{
	if (!DungeonModel::Generate(LevelSize(level), LevelSize(level)))
	{
		DungeonScene::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	else if (!DungeonModel::PlaceCamera())
	{
		DungeonScene::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	DungeonScene::Objects.clear();
	DungeonScene::Containers.resize(1);
	DungeonScene::Lights.resize(1);

	for (size_t indexX = 0; indexX < DungeonMap::Map.size(); ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Map[indexX].size(); ++indexY)
		{
			if (DungeonMap::Map[indexX][indexY] == DungeonTile::ClosedDoor && !DungeonScene::Objects.emplace_back().PlaceDoor(indexX, indexY, level))
			{
				DungeonScene::Destroy();
				return Logger::LogError(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::AscendDoor && !DungeonScene::Objects.emplace_back().PlaceAscent(indexX, indexY, level))
			{
				DungeonScene::Destroy();
				return Logger::LogError(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::DescendDoor && !DungeonScene::Objects.emplace_back().PlaceDescent(indexX, indexY, level))
			{
				DungeonScene::Destroy();
				return Logger::LogError(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::Barrel && !DungeonScene::Containers.emplace_back().Object.PlaceBarrel(indexX, indexY, level))
			{
				DungeonScene::Destroy();
				return Logger::LogError(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonTile::Chest && !DungeonScene::Containers.emplace_back().Object.PlaceChest(indexX, indexY, level))
			{
				DungeonScene::Destroy();
				return Logger::LogError(__FUNCTION__, __LINE__);
			}
		}

	return true;
}

const bool DungeonScene::Initialize()
{
	if (!DungeonScene::Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!Assets::Initialize())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonAssets::Load())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonItem::CreateUnknowns())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonScene::Containers.emplace_back().CreateInventory())
		return Logger::LogError(__FUNCTION__, __LINE__);

	DungeonScene::Lights.emplace_back();
	DungeonScene::Lights.back().Ambient = glm::vec3(0.25f);
	DungeonScene::Lights.back().Diffuse = glm::normalize(glm::vec3(0.6f, 0.35f, 0.35f));
	DungeonScene::Lights.back().Specular = glm::vec3(0.5f);
	DungeonScene::Lights.back().Position = Camera::Position;
	DungeonScene::Lights.back().Constant = 1.0f;
	DungeonScene::Lights.back().Linear = 0.14f;
	DungeonScene::Lights.back().Quadradic = 0.07f;
	DungeonScene::Lights.back().Type = LightType::POINT_LIGHT;

	return true;
}

const bool DungeonScene::Render()
{
	if (DungeonScene::Lights.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!Assets::Models["DungeonModel"].Render(DungeonScene::Lights))
		return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t index = 0; index < DungeonScene::Objects.size(); ++index)
		if (!DungeonScene::Objects[index].Render(DungeonScene::Lights))
			return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t index = 1; index < DungeonScene::Containers.size(); ++index)
		if (!DungeonScene::Containers[index].Object.Render(DungeonScene::Lights))
			return Logger::LogError(__FUNCTION__, __LINE__);

	if (!DungeonInterface::Render())
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool DungeonScene::Update()
{
	const float doorPickDistance = 1.5f, containerPickDistance = 1.75f;

	if (DungeonScene::Lights.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonInterface::Update())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonInterface::Active())
	{
		if (!Camera::Update())
			return Logger::LogError(__FUNCTION__, __LINE__);

		DungeonScene::Lights[0].Position = Camera::Position;

		if (Input::MouseNowPressed(GLFW_MOUSE_BUTTON_2))
		{
			for (size_t index = 0; index < DungeonScene::Objects.size(); ++index)
			{
				if (DungeonScene::Objects[index].Type == ObjectType::WOODEN_DOOR_CLOSED && DungeonScene::Objects[index].Picked(doorPickDistance * DungeonModel::MeshScale) && !DungeonScene::Objects[index].OpenDoor())
					return Logger::LogError(__FUNCTION__, __LINE__);

				else if (DungeonScene::Objects[index].Type == ObjectType::WOODEN_DOOR_OPENED && DungeonScene::Objects[index].Picked(doorPickDistance * DungeonModel::MeshScale) && !DungeonScene::Objects[index].CloseDoor())
					return Logger::LogError(__FUNCTION__, __LINE__);
			}

			for (size_t index = 1; index < DungeonScene::Containers.size(); ++index)
				if (DungeonScene::Containers[index].Object.Picked(containerPickDistance * DungeonModel::MeshScale))
				{
					if (!DungeonInterface::Open(InterfaceType::GAME_CONTAINER_INVENTORY, index))
						return Logger::LogError(__FUNCTION__, __LINE__);

					break;
				}
		}
	}

	return true;
}