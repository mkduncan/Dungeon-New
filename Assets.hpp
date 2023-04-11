#ifndef Assets_H
#define Assets_H

#include "Image.hpp"
#include "ModelMesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <map>

class Assets
{
public:
	static std::map<std::string, ModelMesh> Models;
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;
	static std::map<std::string, Image> Images;

	Assets() = delete;
	~Assets() = delete;
	Assets(const Assets&) = delete;
	Assets(Assets&&) noexcept = delete;
	Assets& operator=(const Assets&) = delete;
	Assets& operator=(Assets&&) noexcept = delete;

	static const bool Destroy();
	static const bool Initialize();
	static const bool LoadModel(const std::string&, const std::string&);
	static const bool LoadShader(const std::string&, const std::string&, const std::string&, const bool = false);
	static const bool LoadTexture(const std::string&, const std::string&);
	static const bool LoadImage(const std::string&, const std::string&);
};

#endif