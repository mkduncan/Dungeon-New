#include "MathManager.hpp"
#include "Texture.hpp"
#include "TraceManager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT
#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_HDR

#include <stb/stb_image.h>

Texture::Texture() : Id(0)
{
}

Texture::~Texture()
{
	if (!this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
}

Texture::Texture(Texture&& texture) noexcept : Id(MathManager::MoveScalar(texture.Id))
{
}

Texture& Texture::operator=(Texture&& texture) noexcept
{
	if (this->Id != 0 && this->Id != texture.Id && !this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);

	this->Id = MathManager::MoveScalar(texture.Id);
	return *this;
}

bool Texture::Load(const std::string& imagePath)
{
	int32_t sizeX = 0, sizeY = 0, sizeZ = 0;
	uint8_t* buffer = nullptr;

	if (this->Id != 0 && !this->Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	buffer = stbi_load(imagePath.c_str(), &sizeX, &sizeY, &sizeZ, 0);

	if (buffer == nullptr)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glGenTextures(1, &this->Id);
	glBindTexture(GL_TEXTURE_2D, this->Id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + sizeZ - 3, sizeX, sizeY, 0, GL_RGB + sizeZ - 3, GL_UNSIGNED_BYTE, buffer);

	stbi_image_free(buffer);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Texture::Enable(const GLint activeID) const
{
	if (this->Id == 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	if (activeID < 32)
	{
		glActiveTexture(GL_TEXTURE0 + activeID);

		if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
			return false;
	}

	else
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glBindTexture(GL_TEXTURE_2D, this->Id);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Texture::Destroy()
{
	if (this->Id != 0)
	{
		glDeleteTextures(1, &this->Id);
		this->Id = 0;

		if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
			return false;
	}

	return true;
}