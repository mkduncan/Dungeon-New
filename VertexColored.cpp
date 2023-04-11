#include "VertexColored.hpp"

VertexColored::VertexColored() : Position(), Color()
{
}

VertexColored::~VertexColored()
{
}

VertexColored::VertexColored(const glm::vec3& position, const glm::vec4& color) : Position(position), Color(color)
{
}

VertexColored::VertexColored(const VertexColored& vertex) : Position(vertex.Position), Color(vertex.Color)
{
}

VertexColored::VertexColored(VertexColored&& vertex) noexcept : Position(vertex.Position), Color(vertex.Color)
{
	vertex.Position = glm::vec3();
	vertex.Color = glm::vec4();
}

VertexColored& VertexColored::operator=(const VertexColored& vertex)
{
	this->Position = vertex.Position;
	this->Color = vertex.Color;

	return (*this);
}

VertexColored& VertexColored::operator=(VertexColored&& vertex) noexcept
{
	this->Position = vertex.Position;
	this->Color = vertex.Color;

	vertex.Position = glm::vec3();
	vertex.Color = glm::vec4();

	return (*this);
}