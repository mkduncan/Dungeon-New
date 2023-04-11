#include "Logger.hpp"
#include "Shader.hpp"

#include <array>
#include <fstream>
#include <glm/glm.hpp>
#include <regex>
#include <sstream>

Shader::Shader() : Id(0)
{
}

Shader::~Shader()
{
	if (!this->Destroy())
		Logger::LogError(__FUNCTION__, __LINE__);
}

Shader::Shader(const Shader& shader) : Id(shader.Id)
{
}

Shader::Shader(Shader&& shader) noexcept : Id(shader.Id)
{
	shader.Id = 0;
}

Shader& Shader::operator=(const Shader& shader)
{
	this->Id = shader.Id;
	return (*this);
}

Shader& Shader::operator=(Shader&& shader) noexcept
{
	if (this->Id != shader.Id)
	{
		if (!this->Destroy())
			Logger::LogError(__FUNCTION__, __LINE__);

		this->Id = shader.Id;
		shader.Id = 0;
	}

	return (*this);
}

const bool Shader::ChangeMaximumLights(const size_t pointLights, const size_t directionalLights, const size_t spotLights)
{
	const std::regex pointLightRegex("#define MAX_POINT_LIGHTS[ ][\\d]", std::regex::optimize);
	const std::regex directionalLightRegex("#define MAX_DIR_LIGHTS[ ][\\d]", std::regex::optimize);
	const std::regex spotLightRegex("#define MAX_SPOT_LIGHTS[ ][\\d]", std::regex::optimize);

	if (pointLights == 0 || directionalLights == 0 || spotLights == 0)
		return Shader::ChangeMaximumLights(glm::max(pointLights, 1ULL), glm::max(directionalLights, 1ULL), glm::max(spotLights, 1ULL));

	Shader::FragmentTexturedLights = std::regex_replace(Shader::FragmentTexturedLights, pointLightRegex, "#define MAX_POINT_LIGHTS " + std::to_string(pointLights));
	Shader::FragmentTexturedLights = std::regex_replace(Shader::FragmentTexturedLights, directionalLightRegex, "#define MAX_DIR_LIGHTS " + std::to_string(directionalLights));
	Shader::FragmentTexturedLights = std::regex_replace(Shader::FragmentTexturedLights, spotLightRegex, "#define MAX_SPOT_LIGHTS " + std::to_string(spotLights));

	return true;
}

const bool Shader::Create(const std::string& vertexCode, const std::string& fragmentCode)
{
	const GLchar* vertexSource = vertexCode.c_str(), * fragmentSource = fragmentCode.c_str();
	std::array<GLchar, 1024> compileStatus;
	GLuint vertexId = 0, fragmentId = 0;
	GLint status = 0;

	if (!this->Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (vertexCode.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (fragmentCode.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vertexSource, nullptr);
	glCompileShader(vertexId);
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &status);

	if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
		return false;

	else if (status == GL_FALSE)
	{
		glGetShaderInfoLog(vertexId, static_cast<GLsizei>(compileStatus.size()), nullptr, &compileStatus[0]);
		glDeleteShader(vertexId);

		Logger::LogErrorGL(__FUNCTION__, __LINE__);
		return Logger::LogError(std::string("\t") + &compileStatus[0]);
	}

	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentId, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentId);
	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &status);

	if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
		return false;

	else if (status == GL_FALSE)
	{
		glGetShaderInfoLog(fragmentId, static_cast<GLsizei>(compileStatus.size()), nullptr, &compileStatus[0]);
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);

		Logger::LogErrorGL(__FUNCTION__, __LINE__);
		return Logger::LogError(std::string("\t") + &compileStatus[0]);
	}

	this->Id = glCreateProgram();
	glAttachShader(this->Id, vertexId);
	glAttachShader(this->Id, fragmentId);
	glLinkProgram(this->Id);
	glGetProgramiv(this->Id, GL_LINK_STATUS, &status);

	if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
		return false;

	else if (status == GL_FALSE)
	{
		glGetProgramInfoLog(this->Id, static_cast<GLsizei>(compileStatus.size()), nullptr, &compileStatus[0]);
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
		glDeleteProgram(this->Id);

		Logger::LogErrorGL(__FUNCTION__, __LINE__);
		return Logger::LogError(std::string("\t") + &compileStatus[0]);
	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::Destroy()
{
	if (this->Id != 0)
	{
		glDeleteProgram(this->Id);
		this->Id = 0;

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
			return false;
	}

	return true;
}

const bool Shader::Enable() const
{
	glUseProgram(this->Id);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::Load(const std::string& vertexFile, const std::string& fragmentFile)
{
	std::ifstream inputFile;
	std::stringstream vertexBuffer, fragmentBuffer;

	inputFile.open(vertexFile);

	if (!inputFile)
		return Logger::LogError(__FUNCTION__, __LINE__);

	vertexBuffer << inputFile.rdbuf();
	inputFile.close();
	inputFile.open(fragmentFile);

	if (!inputFile)
		return Logger::LogError(__FUNCTION__, __LINE__);

	fragmentBuffer << inputFile.rdbuf();
	inputFile.close();

	if (!this->Create(vertexBuffer.str(), fragmentBuffer.str()))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool Shader::UploadBoolean(const std::string& key, const bool value) const
{
	GLint location = 0;

	if (this->Id == 0 || key.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	location = glGetUniformLocation(this->Id, key.c_str());

	if (location == GL_INVALID_VALUE || location == GL_INVALID_OPERATION)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glUniform1i(location, static_cast<GLint>(value));
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::UploadSigned(const std::string& key, const GLint value) const
{
	GLint location = 0;

	if (this->Id == 0 || key.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	location = glGetUniformLocation(this->Id, key.c_str());

	if (location == GL_INVALID_VALUE || location == GL_INVALID_OPERATION)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glUniform1i(location, value);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::UploadReal(const std::string& key, const float value) const
{
	GLint location = 0;

	if (this->Id == 0 || key.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	location = glGetUniformLocation(this->Id, key.c_str());

	if (location == GL_INVALID_VALUE || location == GL_INVALID_OPERATION)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glUniform1f(location, value);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::UploadVector(const std::string& key, const glm::vec2& value) const
{
	GLint location = 0;

	if (this->Id == 0 || key.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	location = glGetUniformLocation(this->Id, key.c_str());

	if (location == GL_INVALID_VALUE || location == GL_INVALID_OPERATION)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glUniform2fv(location, 1, &value[0]);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::UploadVector(const std::string& key, const glm::vec3& value) const
{
	GLint location = 0;

	if (this->Id == 0 || key.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	location = glGetUniformLocation(this->Id, key.c_str());

	if (location == GL_INVALID_VALUE || location == GL_INVALID_OPERATION)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glUniform3fv(location, 1, &value[0]);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::UploadVector(const std::string& key, const glm::vec4& value) const
{
	GLint location = 0;

	if (this->Id == 0 || key.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	location = glGetUniformLocation(this->Id, key.c_str());

	if (location == GL_INVALID_VALUE || location == GL_INVALID_OPERATION)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glUniform4fv(location, 1, &value[0]);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Shader::UploadMatrix(const std::string& key, const glm::mat4& value) const
{
	GLint location = 0;

	if (this->Id == 0 || key.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	location = glGetUniformLocation(this->Id, key.c_str());

	if (location == GL_INVALID_VALUE || location == GL_INVALID_OPERATION)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

std::string Shader::VertexColored = std::string() +
"#version 330 core\n" +
"layout(location = 0) in vec3 aPos;\n" +
"layout(location = 1) in vec4 aColor;\n" +
"out vec4 ourColor;\n" +
"uniform mat4 model;\n" +
"uniform mat4 view;\n" +
"uniform mat4 projection;\n" +
"void main()\n" +
"{\n" +
"gl_Position = projection * view * model * vec4(aPos, 1.0f);\n" +
"ourColor = aColor;\n" +
"}";

std::string Shader::FragmentColored = std::string() +
"#version 330 core\n" +
"out vec4 FragColor;\n" +
"in vec4 ourColor;\n" +
"void main()\n" +
"{\n" +
"FragColor = ourColor;\n" +
"}\n";

std::string Shader::VertexTexturedSimple = std::string() +
"#version 330 core\n" +
"layout(location = 0) in vec3 aPos;\n" +
"layout(location = 1) in vec3 aNormal;\n" +
"layout(location = 2) in vec2 aTexCoord;\n" +
"out vec3 FragPos;\n" +
"out vec3 Normal;\n" +
"out vec2 TexCoords;\n" +
"uniform mat4 model;\n" +
"uniform mat4 view;\n" +
"uniform mat4 projection;\n" +
"void main()\n" +
"{\n" +
"FragPos = vec3(model * vec4(aPos, 1.0));\n" +
"Normal = mat3(transpose(inverse(model))) * aNormal;\n" +
"TexCoords = aTexCoord;\n" +
"gl_Position = projection * view * vec4(FragPos, 1.0);\n" +
"}\n";

std::string Shader::FragmentTexturedSimple = std::string() +
"#version 330 core\n" +
"out vec4 FragColor;\n" +
"in vec3 FragPos;\n" +
"in vec3 Normal;\n" +
"in vec2 TexCoords;\n" +
"uniform sampler2D ourTexture;\n" +
"void main()\n" +
"{\n" +
"FragColor = texture(ourTexture, TexCoords);\n" +
"}\n";

std::string Shader::VertexUI = std::string() +
"#version 330 core\n" +
"layout(location = 0) in vec3 aPos;\n" +
"layout(location = 1) in vec3 aNormal;\n" +
"layout(location = 2) in vec2 aTexCoord;\n" +
"out vec3 FragPos;\n" +
"out vec2 TexCoords;\n" +
"uniform mat4 model;\n" +
"uniform mat4 projection;\n" +
"void main()\n" +
"{\n" +
"FragPos = vec3(vec2(model * vec4(aPos, 1.0)), 0.0);\n" +
"TexCoords = aTexCoord;\n" +
"gl_Position = projection * vec4(FragPos.xyz, 1.0);\n" +
"}";

std::string Shader::FragmentUI = std::string() +
"#version 330 core\n" +
"out vec4 FragColor;\n" +
"in vec3 FragPos;\n" +
"in vec2 TexCoords;\n" +
"uniform sampler2D ourTexture;\n" +
"void main()\n" +
"{\n" +
"FragColor = texture(ourTexture, TexCoords);\n" +
"}\n";

std::string Shader::VertexTexturedLights = std::string() +
"#version 330 core\n" +
"layout(location = 0) in vec3 aPos;\n" +
"layout(location = 1) in vec3 aNormal;\n" +
"layout(location = 2) in vec2 aTexCoord;\n" +
"out vec3 FragPos;\n" +
"out vec3 Normal;\n" +
"out vec2 TexCoords;\n" +
"uniform mat4 model;\n" +
"uniform mat4 view;\n" +
"uniform mat4 projection;\n" +
"void main()\n" +
"{\n" +
"FragPos = vec3(model * vec4(aPos, 1.0));\n" +
"Normal = mat3(transpose(inverse(model))) * aNormal;\n" +
"TexCoords = aTexCoord;\n" +
"gl_Position = projection * view * vec4(FragPos, 1.0);\n" +
"}\n";

std::string Shader::FragmentTexturedLights = std::string() +
"#version 330 core\n" +
"out vec4 FragColor;\n" +
"struct Material\n" +
"{\n" +
"sampler2D diffuse;\n" +
"sampler2D specular;\n" +
"float shininess;\n" +
"};\n" +
"struct DirLight\n" +
"{\n" +
"vec3 direction;\n" +
"vec3 ambient;\n" +
"vec3 diffuse;\n" +
"vec3 specular;\n" +
"};\n" +
"struct PointLight\n" +
"{\n" +
"vec3 position;\n" +
"vec3 ambient;\n" +
"vec3 diffuse;\n" +
"vec3 specular;\n" +
"float constant;\n" +
"float linear;\n" +
"float quadratic;\n" +
"};\n" +
"struct SpotLight\n" +
"{\n" +
"vec3 position;\n" +
"vec3 direction;\n" +
"vec3 ambient;\n" +
"vec3 diffuse;\n" +
"vec3 specular;\n" +
"float cutOff;\n" +
"float outerCutOff;\n" +
"float constant;\n" +
"float linear;\n" +
"float quadratic;\n" +
"};\n" +
"#define MAX_DIR_LIGHTS 1\n" +
"#define MAX_POINT_LIGHTS 1\n" +
"#define MAX_SPOT_LIGHTS 1\n" +
"in vec3 FragPos;\n" +
"in vec3 Normal;\n" +
"in vec2 TexCoords;\n" +
"uniform vec3 viewPos;\n" +
"uniform DirLight dirLights[MAX_DIR_LIGHTS];\n" +
"uniform PointLight pointLights[MAX_POINT_LIGHTS];\n" +
"uniform SpotLight spotLights[MAX_SPOT_LIGHTS];\n" +
"uniform Material material;\n" +
"uniform int NR_DIR_LIGHTS;\n" +
"uniform int NR_POINT_LIGHTS;\n" +
"uniform int NR_SPOT_LIGHTS;\n" +
"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n" +
"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n" +
"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n" +
"void main()\n" +
"{\n" +
"vec3 norm = normalize(Normal);\n" +
"vec3 viewDir = normalize(viewPos - FragPos);\n" +
"vec3 result = vec3(0.0, 0.0, 0.0);\n" +
"for(int i = 0; i < NR_DIR_LIGHTS && i < MAX_DIR_LIGHTS; i++)\n" +
"result += CalcDirLight(dirLights[i], norm, viewDir);\n" +
"for(int i = 0; i < NR_POINT_LIGHTS && i < MAX_POINT_LIGHTS; i++)\n" +
"result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\n" +
"for(int i = 0; i < NR_SPOT_LIGHTS && i < MAX_SPOT_LIGHTS; i++)\n" +
"result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);\n" +
"FragColor = vec4(result, 1.0);\n" +
"}\n" +
"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n" +
"{\n" +
"vec3 lightDir = normalize(-light.direction);\n" +
"float diff = max(dot(normal, lightDir), 0.0);\n" +
"vec3 reflectDir = reflect(-lightDir, normal);\n" +
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n" +
"vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n" +
"vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n" +
"vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n" +
"return (ambient + diffuse + specular);\n" +
"}\n" +
"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n" +
"{\n" +
"vec3 lightDir = normalize(light.position - fragPos);\n" +
"float diff = max(dot(normal, lightDir), 0.0);\n" +
"vec3 reflectDir = reflect(-lightDir, normal);\n" +
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n" +
"float distance = length(light.position - fragPos);\n" +
"float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n" +
"vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n" +
"vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n" +
"vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n" +
"ambient *= attenuation;\n" +
"diffuse *= attenuation;\n" +
"specular *= attenuation;\n" +
"return (ambient + diffuse + specular);\n" +
"}\n" +
"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n" +
"{\n" +
"vec3 lightDir = normalize(light.position - fragPos);\n" +
"float diff = max(dot(normal, lightDir), 0.0);\n" +
"vec3 reflectDir = reflect(-lightDir, normal);\n" +
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n" +
"float distance = length(light.position - fragPos);\n" +
"float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n" +
"float theta = dot(lightDir, normalize(-light.direction));\n" +
"float epsilon = light.cutOff - light.outerCutOff;\n" +
"float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);\n" +
"vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n" +
"vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n" +
"vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n" +
"ambient *= attenuation * intensity;\n" +
"diffuse *= attenuation * intensity;\n" +
"specular *= attenuation * intensity;\n" +
"return (ambient + diffuse + specular);\n" +
"}\n";