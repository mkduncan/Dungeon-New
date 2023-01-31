#ifndef Texture_H
#define Texture_H

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	GLuint Id;

	Texture();
	~Texture();
	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept;

	bool Load(const std::string&);
	bool Enable(const GLint = 0) const;
	bool Destroy();
};

#endif