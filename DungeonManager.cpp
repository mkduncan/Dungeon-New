#include "Camera.hpp"
#include "DungeonData.hpp"
#include "DungeonManager.hpp"
#include "DungeonMap.hpp"
#include "MathManager.hpp"
#include "TraceManager.hpp"

#include <cmath>

std::map<std::string, Model> DungeonManager::Models;
std::map<LightType, std::vector<Light>> DungeonManager::Lights;
std::vector<AlignedBox> DungeonManager::Bounds;
size_t DungeonManager::Level = 0;

static size_t MapSize(const size_t level)
{
	return (level << 3) + 32;
}

bool DungeonManager::Create(const size_t level)
{
	Model dungeonModel, doorModel, barrelModel, chestModel, ladderModel, ladderDownModel, ghostModel;

	if (!DungeonManager::Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!DungeonMap::GenerateModel(MapSize(level) << 1, MapSize(level) << 1, dungeonModel, DungeonManager::Bounds))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!dungeonModel.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Models["DungeonModel"] = std::move(dungeonModel);

	if (!doorModel.Load("Assets/door.obj"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!doorModel.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Models["WoodenDoor"] = std::move(doorModel);

	if (!barrelModel.Load("Assets/barrel.obj"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!barrelModel.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Models["WoodenBarrel"] = std::move(barrelModel);

	if (!chestModel.Load("Assets/chest.obj"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!chestModel.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Models["WoodenChest"] = std::move(chestModel);

	if (!ladderModel.Load("Assets/ladder.obj"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!ladderModel.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Models["WoodenLadder"] = std::move(ladderModel);

	if (!ladderDownModel.Load("Assets/ladder_down.obj"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!ladderDownModel.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Models["WoodenLadderDown"] = std::move(ladderDownModel);

	if (!ghostModel.Load("Assets/ghost.obj"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!ghostModel.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Models["Ghost"] = std::move(ghostModel);

	DungeonManager::Lights[LightType::SPOT_LIGHT].emplace_back();
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Ambient = Vector3(0.25f);
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Diffuse = Vector3(0.75f, 0.25f, 0.25f);
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Specular = Vector3(0.5f);
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Position = Camera::Position;
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Direction = Camera::Front;
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].InnerCutoff = std::cosf(MathManager::Radians(30.0f));
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].OuterCutoff = std::cosf(MathManager::Radians(45.0f));
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Constant = 1.0f;
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Linear = 0.14f;
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Quadradic = 0.07f;
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Type = SPOT_LIGHT;
	DungeonManager::Level = level;

	if (!DungeonMap::BoundCamera())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool DungeonManager::Update()
{
	if (DungeonManager::Models.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (DungeonManager::Lights.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (DungeonManager::Lights[LightType::SPOT_LIGHT].empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (DungeonManager::Bounds.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!Camera::Update(false, false))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	//else if (!DungeonMap::BoundCamera())
		//return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Position = Camera::Position;
	DungeonManager::Lights[LightType::SPOT_LIGHT][0].Direction = Camera::Front;

	return true;
}

//Add support for culling operations with updating the frustum and box-tree used for culling testing.
bool DungeonManager::Render()
{
	AlignedBox doorBox, barrelBox, chestBox, ladderBox, ladderDownBox, ghostBox;
	Matrix4 transform;
	Vector3 size, center, offset;

	if (!DungeonManager::Models["DungeonModel"].Render(DungeonManager::Lights))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	doorBox = DungeonManager::Models["WoodenDoor"].GetBounds();
	barrelBox = DungeonManager::Models["WoodenBarrel"].GetBounds();
	chestBox = DungeonManager::Models["WoodenChest"].GetBounds();
	ladderBox = DungeonManager::Models["WoodenLadder"].GetBounds();
	ladderDownBox = DungeonManager::Models["WoodenLadderDown"].GetBounds();
	ghostBox = DungeonManager::Models["Ghost"].GetBounds();

	for (size_t indexX = 0; indexX < DungeonMap::Map.size(); ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Map[indexX].size(); ++indexY)
		{
			if (DungeonMap::Map[indexX][indexY] == DungeonObject::ClosedDoor)
			{
				size = (doorBox.Maximum - doorBox.Minimum) * 0.5f;
				center = (doorBox.Minimum + doorBox.Maximum) * 0.5f;
				offset = center * -1.0f + Vector3(0.0f, size[1] - DungeonMap::MeshHeight, 0.0f) + Vector3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonMap::MeshScale - Vector3(DungeonMap::MeshScale * 0.5f, 0.0f, DungeonMap::MeshScale * 0.35f);
				transform = Matrix4::Translated(Vector3(center[0], 0.0f, center[2]) + offset - Vector3(size[0] - DungeonMap::MeshScale * 0.5f, 0.0f, size[2] - DungeonMap::MeshScale * 0.5f));
				transform = transform * Matrix4::Translated(Vector3(static_cast<float>(indexX) * DungeonMap::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonMap::MeshScale));

				if (DungeonMap::Map[indexX][indexY - 1] != DungeonObject::Wall && DungeonMap::Map[indexX][indexY + 1] != DungeonObject::Wall)
				{
					transform = transform * Matrix4::Rotated(Vector3(0.0f, MathManager::Radians(90.0f), 0.0f));
					transform = transform * Matrix4::Translated(Vector3(DungeonMap::MeshScale * -0.25f, 0.0f, DungeonMap::MeshScale * 0.5f));
				}

				else
					transform = transform * Matrix4::Translated(Vector3(DungeonMap::MeshScale * 0.5f, 0.0f, DungeonMap::MeshScale * 0.2625f));

				transform = transform * Matrix4::Scaled(Vector3(DungeonMap::MeshScale * 0.05f / size[0], DungeonMap::MeshHeight / size[1], DungeonMap::MeshScale * 0.525f / size[2]));

				if (!DungeonManager::Models["WoodenDoor"].SetTransform(transform))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (!DungeonManager::Models["WoodenDoor"].Render(DungeonManager::Lights))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::Barrel)
			{
				size = (barrelBox.Maximum - barrelBox.Minimum) * 0.5f;
				center = (barrelBox.Minimum + barrelBox.Maximum) * 0.5f;
				offset = center * -1.0f + Vector3(0.0f, size[1] - DungeonMap::MeshHeight, 0.0f) + Vector3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonMap::MeshScale - Vector3(DungeonMap::MeshScale * 0.5f, 0.0f, DungeonMap::MeshScale * 0.35f);
				transform = Matrix4::Translated(Vector3(center[0], 0.0f, center[2]) + offset - Vector3(size[0] - DungeonMap::MeshScale * 0.5f, 0.0f, size[2] - DungeonMap::MeshScale * 0.5f));
				transform = transform * Matrix4::Translated(Vector3(static_cast<float>(indexX) * DungeonMap::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonMap::MeshScale));
				transform = transform * Matrix4::Scaled(Vector3(1.0f / size[0], 1.0f / size[1], 1.0f / size[2]));

				if (!DungeonManager::Models["WoodenBarrel"].SetTransform(transform))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (!DungeonManager::Models["WoodenBarrel"].Render(DungeonManager::Lights))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::Chest)
			{
				size = (chestBox.Maximum - chestBox.Minimum) * 0.5f;
				center = (chestBox.Minimum + chestBox.Maximum) * 0.5f;
				offset = center * -1.0f + Vector3(0.0f, size[1] - DungeonMap::MeshHeight, 0.0f) + Vector3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonMap::MeshScale;
				transform = Matrix4::Translated(Vector3(center[0], 0.0f, center[2]) + offset - Vector3(size[0] - DungeonMap::MeshScale * 0.5f, 0.0f, size[2] - DungeonMap::MeshScale * 0.5f));
				transform = transform * Matrix4::Translated(Vector3(static_cast<float>(indexX) * DungeonMap::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonMap::MeshScale));

				if (DungeonMap::Map[indexX][indexY - 1] == DungeonObject::Unused)
					transform = transform * Matrix4::Rotated(Vector3(0.0f, MathManager::Radians(90.0f), 0.0f));

				else if (DungeonMap::Map[indexX][indexY + 1] == DungeonObject::Unused)
					transform = transform * Matrix4::Rotated(Vector3(0.0f, MathManager::Radians(270.0f), 0.0f));

				else if (DungeonMap::Map[indexX - 1][indexY] == DungeonObject::Unused)
					transform = transform * Matrix4::Rotated(Vector3(0.0f, MathManager::Radians(180.0f), 0.0f));

				transform = transform * Matrix4::Scaled(Vector3(1.0f / size[0], 0.75f / size[1], 1.0f / size[2]));

				if (!DungeonManager::Models["WoodenChest"].SetTransform(transform))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (!DungeonManager::Models["WoodenChest"].Render(DungeonManager::Lights))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::AscendDoor)
			{
				size = (ladderBox.Maximum - ladderBox.Minimum) * 0.5f;
				center = (ladderBox.Minimum + ladderBox.Maximum) * 0.5f;
				offset = center * -1.0f + Vector3(0.0f, size[1] - DungeonMap::MeshHeight, 0.0f) + Vector3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonMap::MeshScale;
				transform = Matrix4::Translated(Vector3(center[0], 0.0f, center[2]) + offset - Vector3(size[0] - DungeonMap::MeshScale * 0.5f, 0.0f, size[2] - DungeonMap::MeshScale * 0.5f));
				transform = transform * Matrix4::Translated(Vector3(static_cast<float>(indexX) * DungeonMap::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonMap::MeshScale));
				transform = transform * Matrix4::Scaled(Vector3(1.0f / size[0], DungeonMap::MeshHeight / size[1], 1.0f / size[2]));

				if (!DungeonManager::Models["WoodenLadder"].SetTransform(transform))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (!DungeonManager::Models["WoodenLadder"].Render(DungeonManager::Lights))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::DescendDoor)
			{
				size = (ladderDownBox.Maximum - ladderDownBox.Minimum) * 0.5f;
				center = (ladderDownBox.Minimum + ladderDownBox.Maximum) * 0.5f;
				offset = center * -1.0f + Vector3(0.0f, size[1] - DungeonMap::MeshHeight, 0.0f) + Vector3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonMap::MeshScale;
				transform = Matrix4::Translated(Vector3(center[0], 0.0f, center[2]) + offset - Vector3(size[0] - DungeonMap::MeshScale * 0.5f, DungeonMap::MeshScale * 0.35f, size[2] - DungeonMap::MeshScale * 0.5f));
				transform = transform * Matrix4::Translated(Vector3(static_cast<float>(indexX) * DungeonMap::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonMap::MeshScale));
				transform = transform * Matrix4::Scaled(Vector3(1.0f / size[0], DungeonMap::MeshHeight / size[1], 1.0f / size[2]));

				if (!DungeonManager::Models["WoodenLadderDown"].SetTransform(transform))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (!DungeonManager::Models["WoodenLadderDown"].Render(DungeonManager::Lights))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);
			}

			else if (DungeonMap::Map[indexX][indexY] == DungeonObject::Enemy)
			{
				size = (ghostBox.Maximum - ghostBox.Minimum) * 0.5f;
				center = (ghostBox.Minimum + ghostBox.Maximum) * 0.5f;

				offset = center * -1.0f + Vector3(0.0f, size[1] - DungeonMap::MeshHeight, 0.0f) + Vector3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonMap::MeshScale - Vector3(DungeonMap::MeshScale * 0.5f, 0.0f, DungeonMap::MeshScale * 0.35f);
				transform = Matrix4::Translated(Vector3(center[0], 0.0f, center[2]) + offset - Vector3(size[0] - DungeonMap::MeshScale * 0.5f, 0.0f, size[2] - DungeonMap::MeshScale * 0.5f));
				transform = transform * Matrix4::Translated(Vector3(DungeonMap::MeshScale * 0.25f, DungeonMap::MeshHeight * -1.25f, DungeonMap::MeshScale * 0.25f));
				transform = transform * Matrix4::Translated(Vector3(static_cast<float>(indexX) * DungeonMap::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonMap::MeshScale));
				transform = transform * Matrix4::Scaled(Vector3(DungeonMap::MeshScale * 0.33333333f / size[0], DungeonMap::MeshHeight * 0.75f / size[1], DungeonMap::MeshScale * 0.25f / size[2]));

				if (!DungeonManager::Models["Ghost"].SetTransform(transform))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (!DungeonManager::Models["Ghost"].Render(DungeonManager::Lights))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);
			}
		}

	return true;
}

bool DungeonManager::Destroy()
{
	bool result = true;

	for (std::map<std::string, Model>::iterator modelIterator = DungeonManager::Models.begin(); modelIterator != DungeonManager::Models.end(); ++modelIterator)
		if (!modelIterator->second.Destroy())
			result = false;

	DungeonManager::Models.clear();

	for (std::map<LightType, std::vector<Light>>::iterator lightIterator = DungeonManager::Lights.begin(); lightIterator != DungeonManager::Lights.end(); ++lightIterator)
		lightIterator->second.clear();

	DungeonManager::Lights.clear();
	DungeonManager::Bounds.clear();
	DungeonManager::Level = 0;

	if (!result)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return result;
}