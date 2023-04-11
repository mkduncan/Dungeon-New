#include "Assets.hpp"
#include "Camera.hpp"
#include "Logger.hpp"
#include "ModelMesh.hpp"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <tinyobjloader/tiny_obj_loader.h>

ModelMesh::ModelMesh() : Meshes(), Materials(), Shader(), Transform(glm::mat4(1.0f)), Bounds()
{
}

ModelMesh::~ModelMesh()
{
	if (!this->Destroy())
		Logger::LogError(__FUNCTION__, __LINE__);
}

ModelMesh::ModelMesh(const ModelMesh& model) : Meshes(model.Meshes), Materials(model.Materials), Shader(model.Shader), Transform(model.Transform), Bounds(model.Bounds)
{
}

ModelMesh::ModelMesh(ModelMesh&& model) noexcept : Meshes(std::move(model.Meshes)), Materials(std::move(model.Materials)), Shader(std::move(model.Shader)), Transform(model.Transform), Bounds(model.Bounds)
{
	model.Transform = glm::mat4(1.0f);
	model.Bounds = AlignedBox();
}

ModelMesh& ModelMesh::operator=(const ModelMesh& model)
{
	this->Meshes = model.Meshes;
	this->Materials = model.Materials;
	this->Shader = model.Shader;
	this->Transform = model.Transform;
	this->Bounds = model.Bounds;

	return (*this);
}

ModelMesh& ModelMesh::operator=(ModelMesh&& model) noexcept
{
	this->Meshes = std::move(model.Meshes);
	this->Materials = std::move(model.Materials);
	this->Shader = std::move(model.Shader);
	this->Transform = model.Transform;
	this->Bounds = model.Bounds;

	model.Transform = glm::mat4(1.0f);
	model.Bounds = AlignedBox();

	return (*this);
}

const bool ModelMesh::Destroy()
{
	bool result = true;

	for (size_t index = 0; index < this->Meshes.size(); ++index)
		if (!this->Meshes[index].Destroy())
		{
			Logger::LogError(__FUNCTION__, __LINE__);
			result = false;
		}

	this->Meshes.clear();
	this->Materials.clear();
	this->Shader.clear();
	this->Transform = glm::mat4(1.0f);
	this->Bounds = AlignedBox();

	return result;
}

const bool ModelMesh::Load(const std::string& inputFile)
{
	tinyobj::ObjReaderConfig config;
	tinyobj::ObjReader reader;
	std::map<size_t, std::vector<VertexTextured>> vertexGroups;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<GLuint> indices;
	tinyobj::attrib_t attributes;
	tinyobj::index_t index;
	size_t pathIndex = inputFile.rfind('/'), groupIndex, offset, vertexCount, vectorIndex, normalIndex, textureIndex;

	if (!this->Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (pathIndex != std::string::npos)
		config.mtl_search_path = inputFile.substr(0, pathIndex + 1);

	if (!reader.ParseFromFile(inputFile, config))
		return Logger::LogError(__FUNCTION__, __LINE__);

	shapes = reader.GetShapes();
	materials = reader.GetMaterials();
	attributes = reader.GetAttrib();

	if (shapes.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (materials.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t shapeIndex = 0; shapeIndex < shapes.size(); ++shapeIndex)
	{
		offset = 0;

		for (size_t faceIndex = 0; faceIndex < shapes[shapeIndex].mesh.num_face_vertices.size(); ++faceIndex)
		{
			groupIndex = shapes[shapeIndex].mesh.material_ids[faceIndex];
			vertexCount = static_cast<size_t>(shapes[shapeIndex].mesh.num_face_vertices[faceIndex]);

			if (groupIndex < 0)
				return Logger::LogError(__FUNCTION__, __LINE__);

			else if (vertexCount != 3)
				return Logger::LogError(__FUNCTION__, __LINE__);

			for (size_t vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
			{
				index = shapes[shapeIndex].mesh.indices[vertexIndex + offset];

				if (index.vertex_index < 0)
					return Logger::LogError(__FUNCTION__, __LINE__);

				else if (index.normal_index < 0)
					return Logger::LogError(__FUNCTION__, __LINE__);

				else if (index.texcoord_index < 0)
					return Logger::LogError(__FUNCTION__, __LINE__);

				vectorIndex = static_cast<size_t>(index.vertex_index) * 3;
				normalIndex = static_cast<size_t>(index.normal_index) * 3;
				textureIndex = (static_cast<size_t>(index.texcoord_index) << 1);

				vertexGroups[groupIndex].emplace_back
				(
					VertexTextured
					(
						glm::vec3(attributes.vertices[vectorIndex + 0], attributes.vertices[vectorIndex + 1], attributes.vertices[vectorIndex + 2]),
						glm::vec3(attributes.normals[normalIndex + 0], attributes.normals[normalIndex + 1], attributes.normals[normalIndex + 2]),
						glm::vec2(attributes.texcoords[textureIndex + 0], attributes.texcoords[textureIndex + 1])
					)
				);
			}

			offset += vertexCount;
		}
	}

	if (!this->Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	this->Meshes.resize(vertexGroups.size());
	this->Materials.resize(vertexGroups.size());

	for (std::map<size_t, std::vector<VertexTextured>>::const_iterator groupIndex = vertexGroups.begin(); groupIndex != vertexGroups.end(); ++groupIndex)
	{
		if (groupIndex->first >= vertexGroups.size())
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		else if (groupIndex->first >= materials.size())
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		else if (materials[groupIndex->first].diffuse_texname.empty())
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		indices.resize(groupIndex->second.size());

		for (size_t meshIndex = 0; meshIndex < groupIndex->second.size(); ++meshIndex)
		{
			indices[meshIndex] = static_cast<GLuint>(meshIndex);

			if (!this->Bounds.Expand(groupIndex->second[meshIndex].Position))
			{
				this->Destroy();
				return Logger::LogError(__FUNCTION__, __LINE__);
			}
		}

		if (!this->Meshes[groupIndex->first].Upload(groupIndex->second, indices))
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		this->Materials[groupIndex->first].DiffuseTexture = config.mtl_search_path + materials[groupIndex->first].diffuse_texname;

		if (!materials[groupIndex->first].specular_texname.empty())
			this->Materials[groupIndex->first].SpecularTexture = config.mtl_search_path + materials[groupIndex->first].specular_texname;

		this->Materials[groupIndex->first].Ambient = glm::make_vec3(materials[groupIndex->first].ambient);
		this->Materials[groupIndex->first].Diffuse = glm::make_vec3(materials[groupIndex->first].diffuse);
		this->Materials[groupIndex->first].Specular = glm::make_vec3(materials[groupIndex->first].specular);
		this->Materials[groupIndex->first].Shininess = materials[groupIndex->first].shininess;
	}

	for (size_t textureIndex = 0; textureIndex < this->Materials.size(); ++textureIndex)
	{
		if (!Assets::LoadTexture(this->Materials[textureIndex].DiffuseTexture, this->Materials[textureIndex].DiffuseTexture))
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		else if (!this->Materials[textureIndex].SpecularTexture.empty() && !Assets::LoadTexture(this->Materials[textureIndex].SpecularTexture, this->Materials[textureIndex].SpecularTexture))
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

	}

	this->Shader = "MeshTextured";
	this->Transform = glm::mat4(1.0f);

	return true;
}

const bool ModelMesh::Render(const std::vector<Light>& lights) const
{
	size_t pointIndex = 0, directionalIndex = 0, spotIndex = 0;
	GLint pointLights = 0, directionalLights = 0, spotLights = 0;

	if (this->Meshes.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (this->Materials.size() != this->Meshes.size())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (this->Shader.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t index = 0; index < lights.size(); ++index)
	{
		if (lights[index].Type == LightType::POINT_LIGHT)
			++pointLights;

		else if (lights[index].Type == LightType::DIRECTIONAL_LIGHT)
			++directionalLights;

		else if (lights[index].Type == LightType::SPOT_LIGHT)
			++spotLights;
	}

	for (size_t index = 0; index < this->Meshes.size(); ++index)
	{
		if (!Assets::Shaders[this->Shader].Enable())
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!Camera::Enable(Assets::Shaders[this->Shader], this->Transform))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!this->Materials[index].Enable(Assets::Shaders[this->Shader]))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!Assets::Shaders[this->Shader].UploadSigned("NR_POINT_LIGHTS", pointLights))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!Assets::Shaders[this->Shader].UploadSigned("NR_DIR_LIGHTS", directionalLights))
			return Logger::LogError(__FUNCTION__, __LINE__);

		else if (!Assets::Shaders[this->Shader].UploadSigned("NR_SPOT_LIGHTS", spotLights))
			return Logger::LogError(__FUNCTION__, __LINE__);

		for (size_t lightIndex = 0; lightIndex < lights.size(); ++lightIndex)
		{
			if (lights[lightIndex].Type == LightType::POINT_LIGHT)
			{
				if (!lights[lightIndex].Enable(Assets::Shaders[this->Shader], pointIndex))
					return Logger::LogError(__FUNCTION__, __LINE__);

				++pointIndex;
			}

			else if (lights[lightIndex].Type == LightType::DIRECTIONAL_LIGHT)
			{
				if (!lights[lightIndex].Enable(Assets::Shaders[this->Shader], directionalIndex))
					return Logger::LogError(__FUNCTION__, __LINE__);

				++directionalIndex;
			}

			else if (lights[lightIndex].Type == LightType::SPOT_LIGHT)
			{
				if (!lights[lightIndex].Enable(Assets::Shaders[this->Shader], spotIndex))
					return Logger::LogError(__FUNCTION__, __LINE__);

				++spotIndex;
			}

			else
				return Logger::LogError(__FUNCTION__, __LINE__);
		}

		if (!this->Meshes[index].Render())
			return Logger::LogError(__FUNCTION__, __LINE__);
	}

	return true;
}

const bool ModelMesh::Upload(const std::vector<std::vector<VertexTextured>>& vertices, const std::vector<std::vector<GLuint>>& indices)
{
	if (!this->Meshes.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (this->Materials.size() != vertices.size())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (this->Shader.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	this->Meshes.resize(vertices.size());
	this->Bounds = AlignedBox();

	for (size_t index = 0; index < this->Meshes.size(); ++index)
	{
		for (size_t meshIndex = 0; meshIndex < vertices[index].size(); ++meshIndex)
			this->Bounds.Expand(vertices[index][meshIndex].Position);

		if (indices.empty() && !this->Meshes[index].Upload(vertices[index]))
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		else if (!indices.empty() && !this->Meshes[index].Upload(vertices[index], indices[index]))
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		else if (!Assets::LoadTexture(this->Materials[index].DiffuseTexture, this->Materials[index].DiffuseTexture))
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}

		else if (!this->Materials[index].SpecularTexture.empty() && !Assets::LoadTexture(this->Materials[index].SpecularTexture, this->Materials[index].SpecularTexture))
		{
			this->Destroy();
			return Logger::LogError(__FUNCTION__, __LINE__);
		}
	}

	return true;
}