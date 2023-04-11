#ifndef Texture_H
#define Texture_H

#include <glad/glad.h>
#include <string>

class Texture
{
private:
	GLuint Id;

public:
	Texture();
	~Texture();
	Texture(const Texture&);
	Texture(Texture&&) noexcept;
	Texture& operator=(const Texture&);
	Texture& operator=(Texture&&) noexcept;

	const bool Destroy();
	const bool Enable(const GLint = 0) const;
	const bool Load(const std::string&);
};

#endif