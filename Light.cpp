#include "Light.hpp"
#include "Logger.hpp"

Light::Light() : Ambient(), Diffuse(), Specular(), Position(), Direction(), InnerCutoff(0.0f), OuterCutoff(0.0f), Constant(0.0f), Linear(0.0f), Quadradic(0.0f), Type(LightType::NULL_LIGHT)
{
}

Light::~Light()
{
}

Light::Light(const Light& light) : Ambient(light.Ambient), Diffuse(light.Diffuse), Specular(light.Specular), Position(light.Position), Direction(light.Direction), InnerCutoff(light.InnerCutoff), OuterCutoff(light.OuterCutoff), Constant(light.Constant), Linear(light.Linear), Quadradic(light.Quadradic), Type(light.Type)
{
}

Light::Light(Light&& light) noexcept : Ambient(light.Ambient), Diffuse(light.Diffuse), Specular(light.Specular), Position(light.Position), Direction(light.Direction), InnerCutoff(light.InnerCutoff), OuterCutoff(light.OuterCutoff), Constant(light.Constant), Linear(light.Linear), Quadradic(light.Quadradic), Type(light.Type)
{
	light.Ambient = glm::vec3();
	light.Diffuse = glm::vec3();
	light.Specular = glm::vec3();
	light.Position = glm::vec3();
	light.Direction = glm::vec3();
	light.InnerCutoff = 0.0f;
	light.OuterCutoff = 0.0f;
	light.Constant = 0.0f;
	light.Linear = 0.0f;
	light.Quadradic = 0.0f;
	light.Type = LightType::NULL_LIGHT;
}

Light& Light::operator=(const Light& light)
{
	this->Ambient = light.Ambient;
	this->Diffuse = light.Diffuse;
	this->Specular = light.Specular;
	this->Position = light.Position;
	this->Direction = light.Direction;
	this->InnerCutoff = light.InnerCutoff;
	this->OuterCutoff = light.OuterCutoff;
	this->Constant = light.Constant;
	this->Linear = light.Linear;
	this->Quadradic = light.Quadradic;
	this->Type = light.Type;

	return *this;
}

Light& Light::operator=(Light&& light) noexcept
{
	this->Ambient = light.Ambient;
	this->Diffuse = light.Diffuse;
	this->Specular = light.Specular;
	this->Position = light.Position;
	this->Direction = light.Direction;
	this->InnerCutoff = light.InnerCutoff;
	this->OuterCutoff = light.OuterCutoff;
	this->Constant = light.Constant;
	this->Linear = light.Linear;
	this->Quadradic = light.Quadradic;
	this->Type = light.Type;

	light.Ambient = glm::vec3();
	light.Diffuse = glm::vec3();
	light.Specular = glm::vec3();
	light.Position = glm::vec3();
	light.Direction = glm::vec3();
	light.InnerCutoff = 0.0f;
	light.OuterCutoff = 0.0f;
	light.Constant = 0.0f;
	light.Linear = 0.0f;
	light.Quadradic = 0.0f;
	light.Type = LightType::NULL_LIGHT;

	return *this;
}

const bool Light::Enable(const Shader& shader, const size_t lightIndex) const
{
	std::string variableName;

	if (!shader.Enable())
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (this->Type == LightType::POINT_LIGHT)
		variableName = "pointLights[" + std::to_string(lightIndex) + "].";

	else if (this->Type == LightType::DIRECTIONAL_LIGHT)
		variableName = "dirLights[" + std::to_string(lightIndex) + "].";

	else if (this->Type == LightType::SPOT_LIGHT)
		variableName = "spotLights[" + std::to_string(lightIndex) + "].";

	else
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (this->Type != LightType::POINT_LIGHT && !shader.UploadVector(variableName + "direction", this->Direction))
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (this->Type != LightType::DIRECTIONAL_LIGHT)
	{
		if(!shader.UploadVector(variableName + "position", this->Position))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!shader.UploadReal(variableName + "constant", this->Constant))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!shader.UploadReal(variableName + "linear", this->Linear))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!shader.UploadReal(variableName + "quadratic", this->Quadradic))
			return Logger::LogError(__FUNCTION__, __LINE__);
	}

	if (this->Type == LightType::SPOT_LIGHT)
	{
		if(!shader.UploadReal(variableName + "cutOff", glm::cos(glm::radians(this->InnerCutoff))))
			return Logger::LogError(__FUNCTION__, __LINE__);
		
		else if(!shader.UploadReal(variableName + "outerCutOff", glm::cos(glm::radians(this->OuterCutoff))))
			return Logger::LogError(__FUNCTION__, __LINE__);
	}

	if (!shader.UploadVector(variableName + "ambient", this->Ambient))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadVector(variableName + "diffuse", this->Diffuse))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadVector(variableName + "specular", this->Specular))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}