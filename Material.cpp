#include "Material.hpp"
#include "MathManager.hpp"
#include "TraceManager.hpp"

Material::Material() : DiffuseTexture(), SpecularTexture(), Ambient(), Diffuse(), Specular(), Shininess(0.0f)
{
}

Material::~Material()
{
	DiffuseTexture.clear();
	SpecularTexture.clear();
}

Material::Material(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float shininess) : DiffuseTexture(), SpecularTexture(), Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess)
{
}

Material::Material(const Material& material) : DiffuseTexture(material.DiffuseTexture), SpecularTexture(material.SpecularTexture), Ambient(material.Ambient), Diffuse(material.Diffuse), Specular(material.Specular), Shininess(material.Shininess)
{
}

Material::Material(Material&& material) noexcept : DiffuseTexture(std::move(material.DiffuseTexture)), SpecularTexture(std::move(material.SpecularTexture)), Ambient(std::move(material.Ambient)), Diffuse(material.Diffuse), Specular(std::move(material.Specular)), Shininess(MathManager::MoveReal(material.Shininess))
{
}

Material& Material::operator=(const Material& material)
{
	this->DiffuseTexture = material.DiffuseTexture;
	this->SpecularTexture = material.SpecularTexture;
	this->Ambient = material.Ambient;
	this->Diffuse = material.Diffuse;
	this->Specular = material.Specular;
	this->Shininess = material.Shininess;

	return *this;
}

Material& Material::operator=(Material&& material) noexcept
{
	this->DiffuseTexture = std::move(material.DiffuseTexture);
	this->SpecularTexture = std::move(material.SpecularTexture);
	this->Ambient = std::move(material.Ambient);
	this->Diffuse = std::move(material.Diffuse);
	this->Specular = std::move(material.Specular);
	this->Shininess = MathManager::MoveReal(material.Shininess);

	return *this;
}

bool Material::Load(const Shader& shader) const
{
	if (!shader.LoadSigned("material.diffuse", 0))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadSigned("material.specular", 1))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadReal("material.shininess", this->Shininess))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}