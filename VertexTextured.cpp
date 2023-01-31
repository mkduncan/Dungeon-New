#include "VertexTextured.hpp"

VertexTextured::VertexTextured() : Position(0.0f), Normal(0.0f), Texture(0.0f)
{
}

VertexTextured::~VertexTextured()
{
}

VertexTextured::VertexTextured(const Vector3& position, const Vector3& normal, const Vector2& texture) : Position(position), Normal(normal), Texture(texture)
{
}

VertexTextured::VertexTextured(const VertexTextured& vertex) : Position(vertex.Position), Normal(vertex.Normal), Texture(vertex.Texture)
{
}

VertexTextured::VertexTextured(VertexTextured&& vertex) noexcept : Position(std::move(vertex.Position)), Normal(std::move(vertex.Normal)), Texture(std::move(vertex.Texture))
{
}

VertexTextured& VertexTextured::operator=(const VertexTextured& vertex)
{
	this->Position = vertex.Position;
	this->Normal = vertex.Normal;
	this->Texture = vertex.Texture;

	return (*this);
}

VertexTextured& VertexTextured::operator=(VertexTextured&& vertex) noexcept
{
	this->Position = std::move(vertex.Position);
	this->Normal = std::move(vertex.Normal);
	this->Texture = std::move(vertex.Texture);

	return (*this);
}