#include "MathManager.hpp"
#include "Shader.hpp"
#include "TraceManager.hpp"

#include <array>
#include <fstream>
#include <sstream>

Shader::Shader() : Id(0)
{
}

Shader::~Shader()
{
	if (!this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
}

Shader::Shader(Shader&& shader) noexcept : Id(MathManager::MoveScalar(shader.Id))
{
}

Shader& Shader::operator=(Shader&& shader) noexcept
{
	if (this->Id != 0 && this->Id != shader.Id && !this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);

	this->Id = MathManager::MoveScalar(shader.Id);
	return (*this);
}

bool Shader::Create(const std::string& vertexCode, const std::string& fragmentCode)
{
	const GLchar* vertexSource = vertexCode.c_str(), * fragmentSource = fragmentCode.c_str();
	std::array<GLchar, 1024> compileStatus;
	GLuint vertexId = 0, fragmentId = 0;
	GLint status = 0;

	if (this->Id != 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (vertexCode.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (fragmentCode.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vertexSource, nullptr);
	glCompileShader(vertexId);
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &status);

	if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
		return false;

	else if (status == GL_FALSE)
	{
		glGetShaderInfoLog(vertexId, static_cast<GLsizei>(compileStatus.size()), nullptr, &compileStatus[0]);
		glDeleteShader(vertexId);

		TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
		return TraceManager::AddMessage(std::string("\t") + &compileStatus[0]);
	}

	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentId, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentId);
	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &status);

	if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
		return false;

	else if (status == GL_FALSE)
	{
		glGetShaderInfoLog(fragmentId, static_cast<GLsizei>(compileStatus.size()), nullptr, &compileStatus[0]);
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);

		TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
		return TraceManager::AddMessage(std::string("\t") + &compileStatus[0]);
	}

	this->Id = glCreateProgram();
	glAttachShader(this->Id, vertexId);
	glAttachShader(this->Id, fragmentId);
	glLinkProgram(this->Id);
	glGetProgramiv(this->Id, GL_LINK_STATUS, &status);

	if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
		return false;

	else if (status == GL_FALSE)
	{
		glGetProgramInfoLog(this->Id, static_cast<GLsizei>(compileStatus.size()), nullptr, &compileStatus[0]);
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
		glDeleteProgram(this->Id);

		TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
		return TraceManager::AddMessage(std::string("\t") + &compileStatus[0]);
	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Shader::Load(const std::string& vertexFile, const std::string& fragmentFile)
{
	std::ifstream inputFile;
	std::stringstream vertexBuffer, fragmentBuffer;

	inputFile.open(vertexFile);

	if (!inputFile)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	vertexBuffer << inputFile.rdbuf();
	inputFile.close();
	inputFile.open(fragmentFile);

	if (!inputFile)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	fragmentBuffer << inputFile.rdbuf();
	inputFile.close();

	if (!this->Create(vertexBuffer.str(), fragmentBuffer.str()))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool Shader::Enable() const
{
	if (this->Id == 0)
	{
		glUseProgram(0);
		return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
	}

	glUseProgram(this->Id);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);;
}

bool Shader::Destroy()
{
	if (this->Id != 0)
	{
		glDeleteProgram(this->Id);
		this->Id = 0;

		if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
			return false;
	}

	return true;
}

bool Shader::LoadBoolean(const std::string& key, const bool value) const
{
	if (this->Id == 0 || key.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glUniform1i(glGetUniformLocation(this->Id, key.c_str()), static_cast<GLint>(value));
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Shader::LoadSigned(const std::string& key, const GLint value) const
{
	if (this->Id == 0 || key.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glUniform1i(glGetUniformLocation(this->Id, key.c_str()), value);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Shader::LoadReal(const std::string& key, const float value) const
{
	if (this->Id == 0 || key.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glUniform1f(glGetUniformLocation(this->Id, key.c_str()), value);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Shader::LoadVector(const std::string& key, const Vector2& value) const
{
	if (this->Id == 0 || key.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glUniform2fv(glGetUniformLocation(this->Id, key.c_str()), 1, &value[0]);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Shader::LoadVector(const std::string& key, const Vector3& value) const
{
	if (this->Id == 0 || key.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glUniform3fv(glGetUniformLocation(this->Id, key.c_str()), 1, &value[0]);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Shader::LoadVector(const std::string& key, const Vector4& value) const
{
	if (this->Id == 0 || key.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glUniform4fv(glGetUniformLocation(this->Id, key.c_str()), 1, &value[0]);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool Shader::LoadMatrix(const std::string& key, const Matrix4& value) const
{
	if (this->Id == 0 || key.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glUniformMatrix4fv(glGetUniformLocation(this->Id, key.c_str()), 1, GL_FALSE, &value[0][0]);
	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}