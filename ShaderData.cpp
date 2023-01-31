#include "ShaderData.hpp"
#include <regex>

std::string ShaderData::VertexColoredSimple = std::string() +
"#version 330 core\n" +
"layout(location = 0) in vec3 aPos;\n" +
"layout(location = 1) in vec3 aColor;\n" +
"out vec3 ourColor;\n" +
"uniform mat4 model;\n" +
"uniform mat4 view;\n" +
"uniform mat4 projection;\n" +
"void main()\n" +
"{\n" +
"gl_Position = projection * view * model * vec4(aPos, 1.0f);\n" +
"ourColor = aColor;\n" +
"}";

std::string ShaderData::VertexTexturedSimple = std::string() +
"#version 330 core\n" +
"layout (location = 0) in vec3 aPos;\n" +
"layout (location = 1) in vec3 aNormal;\n" +
"layout (location = 2) in vec2 aTexCoord;\n" +
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

std::string ShaderData::VertexTexturedLights = ShaderData::VertexTexturedSimple;

std::string ShaderData::FragmentColoredSimple = std::string() +
"#version 330 core\n" +
"out vec4 FragColor;\n" +
"in vec3 ourColor;\n" +
"void main()\n" +
"{\n" +
"FragColor = vec4(ourColor, 1.0f);\n" +
"}\n";

std::string ShaderData::FragmentTexturedSimple = std::string() +
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

std::string ShaderData::FragmentTexturedLights = std::string() +
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

std::string ShaderData::VertexColored()
{
	return ShaderData::VertexColoredSimple;
}

std::string ShaderData::VertexTextured(const bool useLighting)
{
	return !useLighting ? ShaderData::VertexTexturedSimple : ShaderData::VertexTexturedLights;
}

std::string ShaderData::FragmentColored()
{
	return ShaderData::FragmentColoredSimple;
}

std::string ShaderData::FragmentTextured(const bool useLighting)
{
	return !useLighting ? ShaderData::FragmentTexturedSimple : ShaderData::FragmentTexturedLights;
}

bool ShaderData::DefineMaxLights(const size_t maxDirectionalLights, const size_t maxPointLights, const size_t maxSpotLights)
{
	static const std::regex directionalLightRegex("#define MAX_DIR_LIGHTS[ ][\\d]", std::regex::optimize);
	static const std::regex pointLightRegex("#define MAX_POINT_LIGHTS[ ][\\d]", std::regex::optimize);
	static const std::regex spotLightRegex("#define MAX_SPOT_LIGHTS[ ][\\d]", std::regex::optimize);
	
	ShaderData::FragmentTexturedLights = std::regex_replace(ShaderData::FragmentTexturedLights, directionalLightRegex, "#define MAX_DIR_LIGHTS " + std::to_string(maxDirectionalLights));
	ShaderData::FragmentTexturedLights = std::regex_replace(ShaderData::FragmentTexturedLights, pointLightRegex, "#define MAX_POINT_LIGHTS " + std::to_string(maxPointLights));
	ShaderData::FragmentTexturedLights = std::regex_replace(ShaderData::FragmentTexturedLights, spotLightRegex, "#define MAX_SPOT_LIGHTS " + std::to_string(maxSpotLights));

	return true;
}