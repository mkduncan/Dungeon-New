#include "Assets.hpp"
#include "Logger.hpp"
#include "Material.hpp"

Material::Material() : DiffuseTexture(), SpecularTexture(), Ambient(), Diffuse(), Specular(), Shininess(0.0f)
{
}

Material::~Material()
{
}

Material::Material(const Material& material) : DiffuseTexture(material.DiffuseTexture), SpecularTexture(material.SpecularTexture), Ambient(material.Ambient), Diffuse(material.Diffuse), Specular(material.Specular), Shininess(material.Shininess)
{
}

Material::Material(Material&& material) noexcept : DiffuseTexture(std::move(material.DiffuseTexture)), SpecularTexture(std::move(material.SpecularTexture)), Ambient(material.Ambient), Diffuse(material.Diffuse), Specular(material.Specular), Shininess(material.Shininess)
{
	material.Ambient = glm::vec3();
	material.Diffuse = glm::vec3();
	material.Specular = glm::vec3();
	material.Shininess = 0.0f;
}

Material& Material::operator=(const Material& material)
{
	this->DiffuseTexture = material.DiffuseTexture;
	this->SpecularTexture = material.SpecularTexture;
	this->Ambient = material.Ambient;
	this->Diffuse = material.Diffuse;
	this->Specular = material.Specular;
	this->Shininess = material.Shininess;

	return (*this);
}

Material& Material::operator=(Material&& material) noexcept
{
	this->DiffuseTexture = std::move(material.DiffuseTexture);
	this->SpecularTexture = std::move(material.SpecularTexture);
	this->Ambient = material.Ambient;
	this->Diffuse = material.Diffuse;
	this->Specular = material.Specular;
	this->Shininess = material.Shininess;

	material.Ambient = glm::vec3();
	material.Diffuse = glm::vec3();
	material.Specular = glm::vec3();
	material.Shininess = 0.0f;

	return (*this);
}

const bool Material::Enable(const Shader& shader) const
{
	if (!shader.Enable())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadSigned("material.diffuse", 0))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadSigned("material.specular", 1))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadReal("material.shininess", this->Shininess))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!this->DiffuseTexture.empty() && !Assets::Textures[this->DiffuseTexture].Enable())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!this->SpecularTexture.empty() && !Assets::Textures[this->SpecularTexture].Enable(1))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}