#ifndef Material_H
#define Material_H

#include "Shader.hpp"

class Material
{
public:
	std::string DiffuseTexture, SpecularTexture;
	glm::vec3 Ambient, Diffuse, Specular;
	float Shininess;

	Material();
	~Material();
	Material(const Material&);
	Material(Material&&) noexcept;
	Material& operator=(const Material&);
	Material& operator=(Material&&) noexcept;

	const bool Enable(const Shader&) const;
};

#endif