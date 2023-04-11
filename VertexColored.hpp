#ifndef VertexColored_H
#define VertexColored_H

#include <glm/glm.hpp>

class VertexColored
{
public:
	glm::vec3 Position;
	glm::vec4 Color;

	VertexColored();
	~VertexColored();
	VertexColored(const glm::vec3&, const glm::vec4&);
	VertexColored(const VertexColored&);
	VertexColored(VertexColored&&) noexcept;
	VertexColored& operator=(const VertexColored&);
	VertexColored& operator=(VertexColored&&) noexcept;
};

#endif