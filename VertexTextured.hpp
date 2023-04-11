#ifndef VertexTextured_H
#define VertexTextured_H

#include <glm/glm.hpp>

class VertexTextured
{
public:
	glm::vec3 Position, Normal;
	glm::vec2 Texture;

	VertexTextured();
	~VertexTextured();
	VertexTextured(const glm::vec3&, const glm::vec3&, const glm::vec2&);
	VertexTextured(const VertexTextured&);
	VertexTextured(VertexTextured&&) noexcept;
	VertexTextured& operator=(const VertexTextured&);
	VertexTextured& operator=(VertexTextured&&) noexcept;
};

#endif