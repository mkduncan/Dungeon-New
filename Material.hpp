#ifndef Material_H
#define Material_H

#include "Shader.hpp"
#include "Vector3.hpp"

class Material
{
private:
	std::string DiffuseTexture, SpecularTexture;
	Vector3 Ambient, Diffuse, Specular;
	float Shininess;

	friend class Model;

public:
	Material();
	~Material();
	Material(const Vector3&, const Vector3&, const Vector3&, const float);
	Material(const Material&);
	Material(Material&&) noexcept;
	Material& operator=(const Material&);
	Material& operator=(Material&&) noexcept;

	bool Load(const Shader&) const;
};

#endif