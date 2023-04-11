#include "VertexTextured.hpp"

VertexTextured::VertexTextured() : Position(), Normal(), Texture()
{
}

VertexTextured::~VertexTextured()
{
}

VertexTextured::VertexTextured(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texture) : Position(position), Normal(normal), Texture(texture)
{
}

VertexTextured::VertexTextured(const VertexTextured& vertex) : Position(vertex.Position), Normal(vertex.Normal), Texture(vertex.Texture)
{
}

VertexTextured::VertexTextured(VertexTextured&& vertex) noexcept : Position(vertex.Position), Normal(vertex.Normal), Texture(vertex.Texture)
{
	vertex.Position = glm::vec3();
	vertex.Normal = glm::vec3();
	vertex.Texture = glm::vec2();
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
	this->Position = vertex.Position;
	this->Normal = vertex.Normal;
	this->Texture = vertex.Texture;

	vertex.Position = glm::vec3();
	vertex.Normal = glm::vec3();
	vertex.Texture = glm::vec2();

	return (*this);
}