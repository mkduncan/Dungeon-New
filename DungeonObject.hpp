#ifndef DungeonObject_H
#define DungeonObject_H

#include "OrientedBox.hpp"
#include <string>

class DungeonObject
{
public:
	OrientedBox Bounds;
	Matrix4 Transform;
	std::string Name;

	DungeonObject();
	~DungeonObject();
	DungeonObject(const DungeonObject&);
	DungeonObject(DungeonObject&&) noexcept;
	DungeonObject& operator=(const DungeonObject&);
	DungeonObject& operator=(DungeonObject&&) noexcept;

	bool Update(const size_t, const size_t, const std::string&);
	bool Render() const;
};

#endif