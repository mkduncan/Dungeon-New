#include "Light.hpp"
#include "MathManager.hpp"
#include "TraceManager.hpp"

static std::string LightIndexName(const LightType, const size_t, const std::string&);

Light::Light() : Ambient(), Diffuse(), Specular(), Position(), Direction(), InnerCutoff(0.0f), OuterCutoff(0.0f), Constant(0.0f), Linear(0.0f), Quadradic(0.0f), Type(NULL_LIGHT)
{
}

Light::~Light()
{
}

Light::Light(const Light& light) : Ambient(light.Ambient), Diffuse(light.Diffuse), Specular(light.Specular), Position(light.Position), Direction(light.Direction), InnerCutoff(light.InnerCutoff), OuterCutoff(light.OuterCutoff), Constant(light.Constant), Linear(light.Linear), Quadradic(light.Quadradic), Type(light.Type)
{
}

Light::Light(Light&& light) noexcept : Ambient(std::move(light.Ambient)), Diffuse(std::move(light.Diffuse)), Specular(std::move(light.Specular)), Position(std::move(light.Position)), Direction(std::move(light.Direction)), InnerCutoff(MathManager::MoveReal(light.InnerCutoff)), OuterCutoff(MathManager::MoveReal(light.OuterCutoff)), Constant(MathManager::MoveReal(light.Constant)), Linear(MathManager::MoveReal(light.Linear)), Quadradic(MathManager::MoveReal(light.Quadradic)), Type(light.Type)
{
	light.Type = NULL_LIGHT;
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
	this->Ambient = std::move(light.Ambient);
	this->Diffuse = std::move(light.Diffuse);
	this->Specular = std::move(light.Specular);
	this->Position = std::move(light.Position);
	this->Direction = std::move(light.Direction);
	this->InnerCutoff = MathManager::MoveReal(light.InnerCutoff);
	this->OuterCutoff = MathManager::MoveReal(light.OuterCutoff);
	this->Constant = MathManager::MoveReal(light.Constant);
	this->Linear = MathManager::MoveReal(light.Linear);
	this->Quadradic = MathManager::MoveReal(light.Quadradic);
	this->Type = light.Type;
	light.Type = NULL_LIGHT;

	return *this;
}

bool Light::Load(const Shader& shader, const size_t lightIndex) const
{
	if(this->Type == NULL_LIGHT)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if ((this->Type != LightType::DIRECTIONAL_LIGHT) && !shader.LoadVector(LightIndexName(this->Type, lightIndex, "position"), this->Position))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if ((this->Type != LightType::POINT_LIGHT) && !shader.LoadVector(LightIndexName(this->Type, lightIndex, "direction"), this->Direction))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadVector(LightIndexName(this->Type, lightIndex, "ambient"), this->Ambient))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadVector(LightIndexName(this->Type, lightIndex, "diffuse"), this->Diffuse))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadVector(LightIndexName(this->Type, lightIndex, "specular"), this->Specular))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if ((this->Type == LightType::SPOT_LIGHT) && !shader.LoadReal(LightIndexName(this->Type, lightIndex, "cutOff"), cosf(MathManager::Radians(this->InnerCutoff))))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if ((this->Type == LightType::SPOT_LIGHT) && !shader.LoadReal(LightIndexName(this->Type, lightIndex, "outerCutOff"), cosf(MathManager::Radians(this->OuterCutoff))))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if ((this->Type != LightType::DIRECTIONAL_LIGHT) && !shader.LoadReal(LightIndexName(this->Type, lightIndex, "constant"), this->Constant))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if ((this->Type != LightType::DIRECTIONAL_LIGHT) && !shader.LoadReal(LightIndexName(this->Type, lightIndex, "linear"), this->Linear))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if ((this->Type != LightType::DIRECTIONAL_LIGHT) && !shader.LoadReal(LightIndexName(this->Type, lightIndex, "quadratic"), this->Quadradic))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

static std::string LightIndexName(const LightType type, const size_t index, const std::string& variable)
{
	std::string result;

	if (type == LightType::POINT_LIGHT)
		result = "pointLights[";

	else if (type == LightType::DIRECTIONAL_LIGHT)
		result = "dirLights[";

	else if (type == LightType::SPOT_LIGHT)
		result = "spotLights[";

	else
	{
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
		return result;
	}

	result += std::to_string(index);
	result += "].";
	result += variable;

	return result;
}