#include "DungeonObject.hpp"


DungeonObject::DungeonObject() : Bounds(), Transform(), Name()
{
}

DungeonObject::~DungeonObject()
{
}

DungeonObject::DungeonObject(const DungeonObject& object) : Bounds(object.Bounds), Transform(object.Transform), Name(object.Name)
{
}

DungeonObject::DungeonObject(DungeonObject&& object) noexcept : Bounds(std::move(object.Bounds)), Transform(std::move(object.Transform)), Name(std::move(object.Name))
{
}

DungeonObject& DungeonObject::operator=(const DungeonObject& object)
{
	this->Bounds = object.Bounds;
	this->Transform = object.Transform;
	this->Name = object.Name;

	return (*this);
}

DungeonObject& DungeonObject::operator=(DungeonObject&& object) noexcept
{
	this->Bounds = std::move(object.Bounds);
	this->Transform = std::move(object.Transform);
	this->Name = std::move(object.Name);

	return (*this);
}

