#ifndef Shader_H
#define Shader_H

#include "Matrix4.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

#include <glad/glad.h>
#include <string>

class Shader
{
private:
	GLuint Id;

public:
	Shader();
	~Shader();
	Shader(const Shader&) = delete;
	Shader(Shader&&) noexcept;
	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) noexcept;

	bool Create(const std::string&, const std::string&);
	bool Load(const std::string&, const std::string&);
	bool Enable() const;
	bool Destroy();

	bool LoadBoolean(const std::string&, const bool) const;
	bool LoadSigned(const std::string&, const GLint) const;
	bool LoadReal(const std::string&, const float) const;
	bool LoadVector(const std::string&, const Vector2&) const;
	bool LoadVector(const std::string&, const Vector3&) const;
	bool LoadVector(const std::string&, const Vector4&) const;
	bool LoadMatrix(const std::string&, const Matrix4&) const;
};

#endif