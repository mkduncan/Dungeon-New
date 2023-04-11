#include "Assets.hpp"
#include "Logger.hpp"

std::map<std::string, ModelMesh> Assets::Models;
std::map<std::string, Shader> Assets::Shaders;
std::map<std::string, Texture> Assets::Textures;
std::map<std::string, Image> Assets::Images;

const bool Assets::Destroy()
{
	bool result = true;

	for (std::map<std::string, ModelMesh>::iterator modelIterator = Assets::Models.begin(); modelIterator != Assets::Models.end(); ++modelIterator)
		if (!modelIterator->second.Destroy())
		{
			Logger::LogError(__FUNCTION__, __LINE__);
			result = false;
		}

	for (std::map<std::string, Shader>::iterator shaderIterator = Assets::Shaders.begin(); shaderIterator != Assets::Shaders.end(); ++shaderIterator)
		if (!shaderIterator->second.Destroy())
		{
			Logger::LogError(__FUNCTION__, __LINE__);
			result = false;
		}

	for (std::map<std::string, Texture>::iterator textureIterator = Assets::Textures.begin(); textureIterator != Assets::Textures.end(); ++textureIterator)
		if (!textureIterator->second.Destroy())
		{
			Logger::LogError(__FUNCTION__, __LINE__);
			result = false;
		}

	for (std::map<std::string, Image>::iterator imageIterator = Assets::Images.begin(); imageIterator != Assets::Images.end(); ++imageIterator)
		if (!imageIterator->second.Destroy())
		{
			Logger::LogError(__FUNCTION__, __LINE__);
			result = false;
		}

	Assets::Models.clear();
	Assets::Shaders.clear();
	Assets::Textures.clear();
	Assets::Images.clear();

	return result;
}

const bool Assets::Initialize()
{
	if (!Assets::Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!Assets::LoadShader("MeshColored", Shader::VertexColored, Shader::FragmentColored))
	{
		Assets::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	else if (!Assets::LoadShader("MeshTexturedSimple", Shader::VertexTexturedSimple, Shader::FragmentTexturedSimple))
	{
		Assets::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	else if (!Assets::LoadShader("UI", Shader::VertexUI, Shader::FragmentUI))
	{
		Assets::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	else if (!Assets::LoadShader("MeshTextured", Shader::VertexTexturedLights, Shader::FragmentTexturedLights))
	{
		Assets::Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	return true;
}

const bool Assets::LoadModel(const std::string& modelName, const std::string& modelFile)
{
	if (Assets::Models.find(modelName) != Assets::Models.end())
		return true;

	else if (!Assets::Models[modelName].Load(modelFile))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool Assets::LoadShader(const std::string& shaderName, const std::string& vertexShader, const std::string& fragmentShader, const bool loadFiles)
{
	if (Assets::Shaders.find(shaderName) != Assets::Shaders.end())
		return true;

	else if (!loadFiles && !Assets::Shaders[shaderName].Create(vertexShader, fragmentShader))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (loadFiles && !Assets::Shaders[shaderName].Load(vertexShader, fragmentShader))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool Assets::LoadTexture(const std::string& textureName, const std::string& textureFile)
{
	if (Assets::Textures.find(textureName) != Assets::Textures.end())
		return true;

	else if (!Assets::Textures[textureName].Load(textureFile))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool Assets::LoadImage(const std::string& imageName, const std::string& imageFile)
{
	if (Assets::Images.find(imageName) != Assets::Images.end())
		return true;

	else if (!Assets::Images[imageName].Load(imageFile))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}