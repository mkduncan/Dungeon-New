#ifndef VertexTextured_H
#define VertexTextured_H

#include "Vector2.hpp"
#include "Vector3.hpp"

class MeshTextured;

class VertexTextured
{
private:
	Vector3 Position, Normal;
	Vector2 Texture;

	friend class MeshTextured;
	friend class MeshRenderable;

public:
	VertexTextured();
	~VertexTextured();
	VertexTextured(const Vector3&, const Vector3&, const Vector2&);
	VertexTextured(const VertexTextured&);
	VertexTextured(VertexTextured&&) noexcept;
	VertexTextured& operator=(const VertexTextured&);
	VertexTextured& operator=(VertexTextured&&) noexcept;
};

#endif