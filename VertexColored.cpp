#include "VertexColored.hpp"

VertexColored::VertexColored() : Color(0.0f), Position(0.0f)
{
}

VertexColored::~VertexColored()
{
}

VertexColored::VertexColored(const Vector4& color, const Vector3& position) : Color(color), Position(position)
{
}

VertexColored::VertexColored(const VertexColored& vertex) : Color(vertex.Color), Position(vertex.Position)
{
}

VertexColored::VertexColored(VertexColored&& vertex) noexcept : Color(std::move(vertex.Color)), Position(std::move(vertex.Position))
{
}

VertexColored& VertexColored::operator=(const VertexColored& vertex)
{
	this->Color = vertex.Color;
	this->Position = vertex.Position;

	return (*this);
}

VertexColored& VertexColored::operator=(VertexColored&& vertex) noexcept
{
	this->Color = std::move(vertex.Color);
	this->Position = std::move(vertex.Position);

	return (*this);
}