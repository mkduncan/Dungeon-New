#ifndef VertexColored_H
#define VertexColored_H

#include "Vector3.hpp"
#include "Vector4.hpp"

class MeshColored;

class VertexColored
{
public:
	Vector4 Color;
	Vector3 Position;

	VertexColored();
	~VertexColored();
	VertexColored(const Vector4&, const Vector3&);
	VertexColored(const VertexColored&);
	VertexColored(VertexColored&&) noexcept;
	VertexColored& operator=(const VertexColored&);
	VertexColored& operator=(VertexColored&&) noexcept;
};

#endif