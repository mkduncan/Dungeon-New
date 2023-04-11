#ifndef Shader_H
#define Shader_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
private:
	GLuint Id;

public:
	static std::string VertexColored, FragmentColored, VertexTexturedSimple, FragmentTexturedSimple, VertexUI, FragmentUI, VertexTexturedLights, FragmentTexturedLights;

	Shader();
	~Shader();
	Shader(const Shader&);
	Shader(Shader&&) noexcept;
	Shader& operator=(const Shader&);
	Shader& operator=(Shader&&) noexcept;

	static const bool ChangeMaximumLights(const size_t, const size_t, const size_t);
	const bool Create(const std::string&, const std::string&);
	const bool Destroy();
	const bool Enable() const;
	const bool Load(const std::string&, const std::string&);
	const bool UploadBoolean(const std::string&, const bool) const;
	const bool UploadReal(const std::string&, const float) const;
	const bool UploadSigned(const std::string&, const GLint) const;
	const bool UploadVector(const std::string&, const glm::vec2&) const;
	const bool UploadVector(const std::string&, const glm::vec3&) const;
	const bool UploadVector(const std::string&, const glm::vec4&) const;
	const bool UploadMatrix(const std::string&, const glm::mat4&) const;
};

#endif