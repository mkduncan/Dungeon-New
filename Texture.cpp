#include "Logger.hpp"
#include "Texture.hpp"

#include <stb/stb_image.h>

Texture::Texture() : Id(0)
{
}

Texture::~Texture()
{
	if (!this->Destroy())
		Logger::LogError(__FUNCTION__, __LINE__);
}

Texture::Texture(const Texture& texture) : Id(texture.Id)
{
}

Texture::Texture(Texture&& texture) noexcept : Id(texture.Id)
{
	texture.Id = 0;
}

Texture& Texture::operator=(const Texture& texture)
{
	this->Id = texture.Id;
	return (*this);
}

Texture& Texture::operator=(Texture&& texture) noexcept
{
	if (this->Id != texture.Id)
	{
		if (!this->Destroy())
			Logger::LogError(__FUNCTION__, __LINE__);

		this->Id = texture.Id;
		texture.Id = 0;
	}

	return (*this);
}

const bool Texture::Destroy()
{
	if (this->Id != 0)
	{
		glDeleteTextures(1, &this->Id);
		this->Id = 0;

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
			return false;
	}

	return true;
}

const bool Texture::Enable(const GLint activeID) const
{
	if (this->Id == 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (activeID < 32)
	{
		glActiveTexture(GL_TEXTURE0 + activeID);

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
			return false;
	}

	else
		return Logger::LogError(__FUNCTION__, __LINE__);

	glBindTexture(GL_TEXTURE_2D, this->Id);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Texture::Load(const std::string& imagePath)
{
	int32_t sizeX = 0, sizeY = 0, sizeZ = 0;
	uint8_t* buffer = nullptr;

	if (!this->Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	buffer = stbi_load(imagePath.c_str(), &sizeX, &sizeY, &sizeZ, 0);

	if (buffer == nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (sizeZ == 3)
		sizeZ = GL_RGB;

	else if (sizeZ == 4)
		sizeZ = GL_RGBA;

	else
	{
		stbi_image_free(buffer);
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	glGenTextures(1, &this->Id);
	glBindTexture(GL_TEXTURE_2D, this->Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, sizeZ, sizeX, sizeY, 0, sizeZ, GL_UNSIGNED_BYTE, buffer);

	stbi_image_free(buffer);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}