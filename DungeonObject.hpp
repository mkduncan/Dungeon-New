#ifndef DungeonObject_H
#define DungeonObject_H

#include "DungeonMap.hpp"
#include "ModelMesh.hpp"

enum ObjectType
{
	NULL_OBJECT = 0,
	WOODEN_LADDER_UP = 1,
	WOODEN_LADDER_DOWN = 2,
	WOODEN_DOOR_CLOSED = 3,
	WOODEN_DOOR_OPENED = 4,
	WOODEN_BARREL = 5,
	WOODEN_CHEST = 6,
};

class DungeonObject
{
public:
	ModelMesh Model;
	ObjectType Type;
	size_t IndexX, IndexY;

	DungeonObject();
	~DungeonObject();
	DungeonObject(const DungeonObject&);
	DungeonObject(DungeonObject&&) noexcept;
	DungeonObject& operator=(const DungeonObject&);
	DungeonObject& operator=(DungeonObject&&) noexcept;

	const bool Culled() const;
	const bool Picked(const float) const;
	const bool Render(const std::vector<Light>&) const;

	const bool CloseDoor();
	const bool OpenDoor();
	
	const bool PlaceAscent(const size_t, const size_t, const size_t);
	const bool PlaceBarrel(const size_t, const size_t, const size_t);
	const bool PlaceChest(const size_t, const size_t, const size_t);
	const bool PlaceDescent(const size_t, const size_t, const size_t);
	const bool PlaceDoor(const size_t, const size_t, const size_t);

};

#endif