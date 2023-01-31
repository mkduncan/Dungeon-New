#include "Camera.hpp"
#include "MeshRenderable.hpp"
#include "TraceManager.hpp"

MeshRenderable::MeshRenderable() : SubMeshColored(), SubMeshTextured(), SubShader(), SubTexture(), SubMaterial()
{
}

MeshRenderable::~MeshRenderable()
{
	if (!this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
}

MeshRenderable::MeshRenderable(MeshRenderable&& mesh) noexcept : SubMeshColored(std::move(mesh.SubMeshColored)), SubMeshTextured(std::move(mesh.SubMeshTextured)), SubShader(std::move(mesh.SubShader)), SubTexture(std::move(mesh.SubTexture)), SubMaterial(std::move(mesh.SubMaterial))
{
}

MeshRenderable& MeshRenderable::operator=(MeshRenderable&& mesh) noexcept
{
	this->SubMeshColored = std::move(mesh.SubMeshColored);
	this->SubMeshTextured = std::move(mesh.SubMeshTextured);
	this->SubShader = std::move(mesh.SubShader);
	this->SubTexture = std::move(mesh.SubTexture);
	this->SubMaterial = std::move(mesh.SubMaterial);

	return (*this);
}

bool MeshRenderable::AddVertex(const VertexColored& vertex)
{
	if (!this->SubMeshTextured.Empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return this->SubMeshColored.AddVertex(vertex);
}

bool MeshRenderable::AddVertex(const VertexTextured& vertex)
{
	if (!this->SubMeshColored.Empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return this->SubMeshTextured.AddVertex(vertex);
}

bool MeshRenderable::LoadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const bool loadFromFile)
{
	if (!this->SubShader.Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (loadFromFile && !this->SubShader.Load(vertexShaderPath, fragmentShaderPath))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!loadFromFile && !this->SubShader.Create(vertexShaderPath, fragmentShaderPath))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool MeshRenderable::LoadTexture(const std::string& diffuseTexturePath)
{
	if (!this->SubTexture.Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!this->SubTexture.Load(diffuseTexturePath))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool MeshRenderable::LoadMaterial(const Material& material)
{
	this->SubMaterial = material;
	return true;
}

bool MeshRenderable::SetIndices(const std::vector<GLuint>& indices)
{
	if (!this->SubMeshColored.Empty() && !this->SubMeshTextured.Empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!this->SubMeshColored.Empty() && !this->SubMeshColored.SetIndices(indices))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!this->SubMeshTextured.Empty() && !this->SubMeshTextured.SetIndices(indices))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool MeshRenderable::Upload()
{
	if (!this->SubMeshColored.Empty() && !this->SubMeshTextured.Empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!this->SubMeshColored.Empty() && !this->SubMeshColored.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!this->SubMeshTextured.Empty() && !this->SubMeshTextured.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool MeshRenderable::Render(const Matrix4& transform, const std::map<LightType, std::vector<Light>>& lights) const
{
	std::map<LightType, std::vector<Light>>::const_iterator lightIterator;
	size_t pointLights = 0, directionalLights = 0, spotLights = 0;

	if (!this->SubMeshColored.Empty())
	{
		if (!this->SubShader.Enable())
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!Camera::LoadWorldMatrix(this->SubShader, transform))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!this->SubMeshColored.Render())
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);
	}

	else if (!this->SubMeshTextured.Empty())
	{
		if (!this->SubShader.Enable())
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!Camera::LoadWorldMatrix(this->SubShader, transform))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!this->SubMaterial.Load(this->SubShader))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!this->SubTexture.Enable())
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		for (lightIterator = lights.begin(); lightIterator != lights.end(); ++lightIterator)
		{
			if (lightIterator->first == LightType::POINT_LIGHT)
				pointLights = lightIterator->second.size();

			else if (lightIterator->first == LightType::DIRECTIONAL_LIGHT)
				directionalLights = lightIterator->second.size();

			else if (lightIterator->first == LightType::SPOT_LIGHT)
				spotLights = lightIterator->second.size();
		}

		if (!this->SubShader.LoadSigned("NR_POINT_LIGHTS", static_cast<GLint>(pointLights)))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!this->SubShader.LoadSigned("NR_DIR_LIGHTS", static_cast<GLint>(directionalLights)))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!this->SubShader.LoadSigned("NR_SPOT_LIGHTS", static_cast<GLint>(spotLights)))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		for (lightIterator = lights.begin(); lightIterator != lights.end(); ++lightIterator)
			for (size_t index = 0; index < lightIterator->second.size(); ++index)
				if (!lightIterator->second[index].Load(this->SubShader, index))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		if (!this->SubMeshTextured.Render())
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);
	}

	else
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool MeshRenderable::Destroy()
{
	std::array<bool, 4> results = { this->SubMeshColored.Destroy(), this->SubMeshTextured.Destroy(), this->SubShader.Destroy(), this->SubTexture.Destroy() };

	for (size_t index = 0; index < results.size(); ++index)
		if (!results[index])
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

const AlignedBox MeshRenderable::GetBounds() const
{
	AlignedBox result;

	if (!this->SubMeshColored.Empty())
		for (size_t index = 0; index < this->SubMeshColored.Vertices.size(); ++index)
			result.Envelope(this->SubMeshColored.Vertices[index].Position);

	else if (!this->SubMeshTextured.Empty())
		for (size_t index = 0; index < this->SubMeshTextured.Vertices.size(); ++index)
			result.Envelope(this->SubMeshTextured.Vertices[index].Position);

	return result;
}