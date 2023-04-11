#ifndef Image_H
#define Image_H

#include "MeshTextured.hpp"
#include "Texture.hpp"

class Image
{
public:
	MeshTextured Mesh;
	Texture Texture;
	glm::mat4 Model, Projection;

	Image();
	~Image();
	Image(const Image&);
	Image(Image&&) noexcept;
	Image& operator=(const Image&);
	Image& operator=(Image&&) noexcept;

	const bool Destroy();
	const bool Load(const std::string&);
	const bool Render() const;

	const bool Position(const glm::vec2&, const bool = false);
	const bool Position(const glm::vec2&, const glm::vec2&, const bool = false);
	const bool Rotate(const float, const bool = false);
	const bool Scale(const glm::vec2&, const bool = false);
	const bool Scale(const glm::vec2&, const glm::vec2&, const bool = false);
};

#endif