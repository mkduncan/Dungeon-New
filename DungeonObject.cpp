#include "Assets.hpp"
#include "Camera.hpp"
#include "DungeonModel.hpp"
#include "DungeonObject.hpp"
#include "Frustum.hpp"
#include "Logger.hpp"
#include "Ray.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <limits>

DungeonObject::DungeonObject() : Model(), Type(ObjectType::NULL_OBJECT), IndexX(0), IndexY(0)
{
}

DungeonObject::~DungeonObject()
{
	this->Model = ModelMesh();
	this->Type = ObjectType::NULL_OBJECT;
	this->IndexX = 0;
	this->IndexY = 0;
}

DungeonObject::DungeonObject(const DungeonObject& object) : Model(object.Model), Type(object.Type), IndexX(object.IndexX), IndexY(object.IndexY)
{
}

DungeonObject::DungeonObject(DungeonObject&& object) noexcept : Model(std::move(object.Model)), Type(object.Type), IndexX(object.IndexX), IndexY(object.IndexY)
{
	object.Type = ObjectType::NULL_OBJECT;
}

DungeonObject& DungeonObject::operator=(const DungeonObject& object)
{
	this->Model = object.Model;
	this->Type = object.Type;
	this->IndexX = object.IndexX;
	this->IndexY = object.IndexY;

	return (*this);
}

DungeonObject& DungeonObject::operator=(DungeonObject&& object) noexcept
{
	this->Model = std::move(object.Model);
	this->Type = object.Type;
	this->IndexX = object.IndexX;
	this->IndexY = object.IndexY;

	object.Type = ObjectType::NULL_OBJECT;
	object.IndexX = 0;
	object.IndexY = 0;

	return (*this);
}

const bool DungeonObject::Culled() const
{
	return Frustum::Culled(this->Model);
}

const bool DungeonObject::Picked(const float distance) const
{
	float result = std::numeric_limits<float>::max();

	if (Ray::Picked(this->Model, result) && result <= distance)
		return true;

	return false;
}

const bool DungeonObject::Render(const std::vector<Light>& lights) const
{
	if (this->Type == ObjectType::NULL_OBJECT)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!this->Culled() && !this->Model.Render(lights))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool DungeonObject::CloseDoor()
{
	glm::vec3 size, center, offset;
	int32_t currentCameraIndexX = 0, currentCameraIndexY = 0;

	if (this->Type != ObjectType::WOODEN_DOOR_OPENED)
		return Logger::LogError(__FUNCTION__, __LINE__);

	currentCameraIndexX = static_cast<int32_t>(Camera::Position.x / DungeonModel::MeshScale);
	currentCameraIndexY = static_cast<int32_t>(Camera::Position.z / DungeonModel::MeshScale);

	if ((currentCameraIndexX == this->IndexX && currentCameraIndexY == this->IndexY))
		return true;

	size = this->Model.Bounds.Maximum - this->Model.Bounds.Minimum;
	center = (this->Model.Bounds.Minimum + this->Model.Bounds.Maximum) * 0.5f;
	offset = center - glm::vec3(DungeonModel::MeshScale * 0.25f, 0.0f, 0.0f) + glm::vec3(size.x, 0.0f, 0.0);

	this->Model.Transform = this->Model.Transform * glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->Model.Transform = this->Model.Transform * glm::translate(glm::mat4(1.0f), -offset);
	this->Model.Transform = this->Model.Transform * glm::rotate(glm::mat4(1.0f), glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->Model.Transform = this->Model.Transform * glm::translate(glm::mat4(1.0f), offset);
	this->Type = ObjectType::WOODEN_DOOR_CLOSED;

	DungeonMap::Map[this->IndexX][this->IndexY] = DungeonTile::ClosedDoor;
	return true;
}

const bool DungeonObject::OpenDoor()
{
	glm::vec3 size, center, offset;

	if (this->Type != ObjectType::WOODEN_DOOR_CLOSED)
		return Logger::LogError(__FUNCTION__, __LINE__);

	size = this->Model.Bounds.Maximum - this->Model.Bounds.Minimum;
	center = (this->Model.Bounds.Minimum + this->Model.Bounds.Maximum) * 0.5f;
	offset = center - glm::vec3(DungeonModel::MeshScale * 0.25f, 0.0f, 0.0f) + glm::vec3(size.x, 0.0f, 0.0);

	this->Model.Transform = this->Model.Transform * glm::translate(glm::mat4(1.0f), -offset);
	this->Model.Transform = this->Model.Transform * glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->Model.Transform = this->Model.Transform * glm::translate(glm::mat4(1.0f), offset);
	this->Model.Transform = this->Model.Transform * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->Type = ObjectType::WOODEN_DOOR_OPENED;

	DungeonMap::Map[this->IndexX][this->IndexY] = DungeonTile::OpenedDoor;
	return true;
}

const bool DungeonObject::PlaceAscent(const size_t indexX, const size_t indexY, const size_t level)
{
	AlignedBox bounds;
	glm::mat4 transform;
	glm::vec3 size, center, offset;

	if (DungeonMap::Map[indexX][indexY] != DungeonTile::AscendDoor)
		return Logger::LogError(__FUNCTION__, __LINE__);

	bounds = Assets::Models["WoodenLadderUp"].Bounds;
	size = (bounds.Maximum - bounds.Minimum) * 0.5f;
	center = (bounds.Minimum + bounds.Maximum) * 0.5f;
	offset = center * -1.0f + glm::vec3(0.0f, size[1] - DungeonModel::MeshHeight, 0.0f) + glm::vec3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonModel::MeshScale;
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(center[0], 0.0f, center[2]) + offset - glm::vec3(size[0] - DungeonModel::MeshScale * 0.5f, 0.0f, size[2] - DungeonModel::MeshScale * 0.5f));
	transform = transform * glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(indexX) * DungeonModel::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonModel::MeshScale));
	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / size[0], DungeonModel::MeshHeight / size[1], 1.0f / size[2]));

	this->Model = Assets::Models["WoodenLadderUp"];
	this->Type = ObjectType::WOODEN_LADDER_UP;
	this->Model.Transform = transform;
	this->IndexX = indexX;
	this->IndexY = indexY;

	return true;
}

const bool DungeonObject::PlaceBarrel(const size_t indexX, const size_t indexY, const size_t level)
{
	AlignedBox bounds;
	glm::mat4 transform;
	glm::vec3 size, center, offset;

	if (DungeonMap::Map[indexX][indexY] != DungeonTile::Barrel)
		return Logger::LogError(__FUNCTION__, __LINE__);

	bounds = Assets::Models["WoodenBarrel"].Bounds;
	size = (bounds.Maximum - bounds.Minimum) * 0.5f;
	center = (bounds.Minimum + bounds.Maximum) * 0.5f;
	offset = center * -1.0f + glm::vec3(0.0f, size[1] - DungeonModel::MeshHeight, 0.0f) + glm::vec3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonModel::MeshScale - glm::vec3(DungeonModel::MeshScale * 0.5f, 0.0f, DungeonModel::MeshScale * 0.35f);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(center[0], 0.0f, center[2]) + offset - glm::vec3(size[0] - DungeonModel::MeshScale * 0.5f, 0.0f, size[2] - DungeonModel::MeshScale * 0.5f));
	transform = transform * glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(indexX) * DungeonModel::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonModel::MeshScale));
	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / size[0], 1.0f / size[1], 1.0f / size[2]));

	this->Model = Assets::Models["WoodenBarrel"];
	this->Type = ObjectType::WOODEN_BARREL;
	this->Model.Transform = transform;
	this->IndexX = indexX;
	this->IndexY = indexY;

	return true;
}

const bool DungeonObject::PlaceChest(const size_t indexX, const size_t indexY, const size_t level)
{
	AlignedBox bounds;
	glm::mat4 transform;
	glm::vec3 size, center, offset;

	if (DungeonMap::Map[indexX][indexY] != DungeonTile::Chest)
		return Logger::LogError(__FUNCTION__, __LINE__);

	bounds = Assets::Models["WoodenChest"].Bounds;
	size = (bounds.Maximum - bounds.Minimum) * 0.5f;
	center = (bounds.Minimum + bounds.Maximum) * 0.5f;
	offset = center * -1.0f + glm::vec3(0.0f, size[1] - DungeonModel::MeshHeight, 0.0f) + glm::vec3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonModel::MeshScale;
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(center[0], 0.0f, center[2]) + offset - glm::vec3(size[0] - DungeonModel::MeshScale * 0.5f, 0.0f, size[2] - DungeonModel::MeshScale * 0.5f));
	transform = transform * glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(indexX) * DungeonModel::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonModel::MeshScale));

	if (DungeonMap::Map[indexX][indexY - 1] == DungeonTile::Unused)
		transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	else if (DungeonMap::Map[indexX][indexY + 1] == DungeonTile::Unused)
		transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	else if (DungeonMap::Map[indexX - 1][indexY] == DungeonTile::Unused)
		transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / size[0], 0.75f / size[1], 1.0f / size[2]));

	this->Model = Assets::Models["WoodenChest"];
	this->Type = ObjectType::WOODEN_CHEST;
	this->Model.Transform = transform;
	this->IndexX = indexX;
	this->IndexY = indexY;

	return true;
}

const bool DungeonObject::PlaceDescent(const size_t indexX, const size_t indexY, const size_t level)
{
	AlignedBox bounds;
	glm::mat4 transform;
	glm::vec3 size, center, offset;

	if (DungeonMap::Map[indexX][indexY] != DungeonTile::DescendDoor)
		return Logger::LogError(__FUNCTION__, __LINE__);

	bounds = Assets::Models["WoodenLadderDown"].Bounds;
	size = (bounds.Maximum - bounds.Minimum) * 0.5f;
	center = (bounds.Minimum + bounds.Maximum) * 0.5f;
	offset = center * -1.0f + glm::vec3(0.0f, size[1] - DungeonModel::MeshHeight, 0.0f) + glm::vec3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonModel::MeshScale;
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(center[0], 0.0f, center[2]) + offset - glm::vec3(size[0] - DungeonModel::MeshScale * 0.5f, DungeonModel::MeshScale * 0.35f, size[2] - DungeonModel::MeshScale * 0.5f));
	transform = transform * glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(indexX) * DungeonModel::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonModel::MeshScale));
	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / size[0], DungeonModel::MeshHeight / size[1], 1.0f / size[2]));

	this->Model = Assets::Models["WoodenLadderDown"];
	this->Type = ObjectType::WOODEN_LADDER_DOWN;
	this->Model.Transform = transform;
	this->IndexX = indexX;
	this->IndexY = indexY;

	return true;
}

const bool DungeonObject::PlaceDoor(const size_t indexX, const size_t indexY, const size_t level)
{
	AlignedBox bounds;
	glm::mat4 transform;
	glm::vec3 size, center, offset;

	if (DungeonMap::Map[indexX][indexY] != DungeonTile::ClosedDoor)
		return Logger::LogError(__FUNCTION__, __LINE__);

	bounds = Assets::Models["WoodenDoor"].Bounds;
	size = (bounds.Maximum - bounds.Minimum) * 0.5f;
	center = (bounds.Minimum + bounds.Maximum) * 0.5f;
	offset = center * -1.0f + glm::vec3(0.0f, size[1] - DungeonModel::MeshHeight, 0.0f) + glm::vec3(size[0] * 0.5f, 0.0f, size[2] * 0.5f) * DungeonModel::MeshScale - glm::vec3(DungeonModel::MeshScale * 0.5f, 0.0f, DungeonModel::MeshScale * 0.35f);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(center[0], 0.0f, center[2]) + offset - glm::vec3(size[0] - DungeonModel::MeshScale * 0.5f, 0.0f, size[2] - DungeonModel::MeshScale * 0.5f));
	transform = transform * glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(indexX) * DungeonModel::MeshScale, 0.0f, static_cast<float>(indexY) * DungeonModel::MeshScale));

	if (DungeonMap::Map[indexX][indexY - 1] != DungeonTile::Wall && DungeonMap::Map[indexX][indexY + 1] != DungeonTile::Wall)
	{
		transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = transform * glm::translate(glm::mat4(1.0f), glm::vec3(DungeonModel::MeshScale * -0.75f, 0.0f, DungeonModel::MeshScale * 0.5f));
	}

	else
	{
		transform = transform * glm::translate(glm::mat4(1.0f), glm::vec3(DungeonModel::MeshScale, 0.0f, DungeonModel::MeshScale * 0.2625f));
		transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(DungeonModel::MeshScale * 0.05f / size[0], DungeonModel::MeshHeight / size[1], DungeonModel::MeshScale * 0.525f / size[2]));

	this->Model = Assets::Models["WoodenDoor"];
	this->Type = ObjectType::WOODEN_DOOR_CLOSED;
	this->Model.Transform = transform;
	this->IndexX = indexX;
	this->IndexY = indexY;

	return true;
}