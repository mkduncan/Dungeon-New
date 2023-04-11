#ifndef AlignedBox_H
#define AlignedBox_H

#include <glm/glm.hpp>

class AlignedBox
{
public:
	glm::vec3 Minimum, Maximum;

	AlignedBox();
	~AlignedBox();
	AlignedBox(const glm::vec3&, const glm::vec3&);
	AlignedBox(const AlignedBox&);
	AlignedBox(AlignedBox&&) noexcept;
	AlignedBox& operator=(const AlignedBox&);
	AlignedBox& operator=(AlignedBox&&) noexcept;

	const bool Expand(const glm::vec3&);
	const bool Render(const glm::mat4&, const glm::vec4& = glm::vec4(1.0f)) const;
};

#endif