#include "Model.hpp"
#include "ShaderData.hpp"
#include "TraceManager.hpp"

#include <fstream>
#include <sstream>

Model::Model() : Meshes(), Transform()
{
}

Model::~Model()
{
	if (!this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
}

Model::Model(Model&& model) noexcept : Meshes(std::move(model.Meshes)), Transform(std::move(model.Transform))
{
}

Model::Model(MeshRenderable&& mesh) noexcept : Meshes(), Transform()
{
	this->Meshes.push_back(std::move(mesh));
}

Model& Model::operator=(Model&& model) noexcept
{
	if (!this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else
	{
		this->Meshes = std::move(model.Meshes);
		this->Transform = std::move(model.Transform);
	}

	return (*this);
}

Model& Model::operator=(MeshRenderable&& mesh) noexcept
{
	if (!this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else
	{
		this->Meshes.push_back(std::move(mesh));
		this->Transform = Matrix4();
	}

	return (*this);
}

const Matrix4& Model::GetTransform() const
{
	return this->Transform;
}

bool Model::SetTransform(const Matrix4& transform)
{
	this->Transform = transform;
	return true;
}

bool Model::Add(MeshRenderable&& mesh)
{
	this->Meshes.push_back(std::move(mesh));
	return true;
}

bool Model::Load(const std::string& modelFile)
{
	std::ifstream inputFile;
	std::stringstream inputBuffer, lineBuffer;
	std::map<std::string, MeshRenderable> meshMap;
	std::map<std::string, Material> materialMap;
	std::vector<Vector3> Vertices, Normals;
	std::vector<Vector2> Coordinates;
	std::string directoryPath, inputLine, inputToken, faceToken[3];
	std::map<std::string, MeshRenderable>::iterator meshIterator = meshMap.end();
	size_t faceIndex[3] = { 0, 0, 0 }, directoryIndex;

	if (!this->Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	directoryIndex = modelFile.rfind('/');

	if (directoryIndex != std::string::npos)
		directoryPath = modelFile.substr(0, directoryIndex + 1);

	inputFile.open(modelFile);

	if (!inputFile)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	inputBuffer << inputFile.rdbuf();
	inputFile.close();

	while (std::getline(inputBuffer, inputLine))
	{
		if (inputLine.empty())
			continue;

		lineBuffer = std::stringstream();
		lineBuffer << inputLine;
		lineBuffer >> inputToken;

		if (inputToken == "mtllib")
		{
			lineBuffer >> inputToken;

			if (!lineBuffer.eof() || lineBuffer.fail())
				return TraceManager::AddMessage(__FUNCTION__, __LINE__);

			materialMap = ParseMaterialFile(directoryPath, inputToken);

			if (materialMap.empty())
				return TraceManager::AddMessage(__FUNCTION__, __LINE__);
		}

		else if (inputToken == "usemtl")
		{
			lineBuffer >> inputToken;
			meshIterator = meshMap.find(inputToken);

			if (meshIterator == meshMap.end())
				meshIterator = meshMap.insert(meshMap.begin(), std::make_pair(inputToken, MeshRenderable()));
		}

		else if (inputToken == "v")
		{
			Vertices.emplace_back();
			lineBuffer >> Vertices.back()[0] >> Vertices.back()[1] >> Vertices.back()[2];
		}

		else if (inputToken == "vn")
		{
			Normals.emplace_back();
			lineBuffer >> Normals.back()[0] >> Normals.back()[1] >> Normals.back()[2];
		}

		else if (inputToken == "vt")
		{
			Coordinates.emplace_back();
			lineBuffer >> Coordinates.back()[0] >> Coordinates.back()[1];
		}

		else if (inputToken == "f")
		{
			if (meshIterator == meshMap.end())
				return TraceManager::AddMessage(__FUNCTION__, __LINE__);

			lineBuffer >> faceToken[0] >> faceToken[1] >> faceToken[2];

			for (size_t index = 0; index < 3; ++index)
			{
				lineBuffer = std::stringstream();
				lineBuffer << faceToken[index];

				for (size_t split = 0; split < 3; ++split)
				{
					std::getline(lineBuffer, inputToken, '/');
					faceIndex[split] = std::stoull(inputToken) - 1;
				}

				if (faceIndex[0] >= Vertices.size())
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (faceIndex[2] >= Normals.size())
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (faceIndex[1] >= Coordinates.size())
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);

				else if (!meshIterator->second.SubMeshTextured.AddVertex(VertexTextured(Vertices[faceIndex[0]], Normals[faceIndex[2]], Coordinates[faceIndex[1]])))
					return TraceManager::AddMessage(__FUNCTION__, __LINE__);
			}
		}

		else if (!lineBuffer.fail())
			continue;

		if (!lineBuffer.eof() || lineBuffer.fail())
		{
			materialMap.clear();
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);
		}
	}

	for (meshIterator = meshMap.begin(); meshIterator != meshMap.end(); ++meshIterator)
	{
		if (!meshIterator->second.LoadShaders(ShaderData::VertexTextured(), ShaderData::FragmentTextured()))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!meshIterator->second.LoadMaterial(materialMap[meshIterator->first]))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		else if (!meshIterator->second.LoadTexture(meshIterator->second.SubMaterial.DiffuseTexture))
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		this->Meshes.push_back(std::move(meshIterator->second));
	}

	return true;
}

bool Model::Upload()
{
	for (size_t index = 0; index < this->Meshes.size(); ++index)
		if (!this->Meshes[index].Upload())
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool Model::Render(const std::map<LightType, std::vector<Light>>& lights) const
{
	bool result = true;

	for (size_t index = 0; index < this->Meshes.size(); ++index)
		if (!this->Meshes[index].Render(this->Transform, lights))
			result = false;

	if(!result)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return result;
}

bool Model::Destroy()
{
	bool result = true;

	for (size_t index = 0; index < this->Meshes.size(); ++index)
		if (!this->Meshes[index].Destroy())
			result = false;

	this->Meshes.clear();
	this->Transform = Matrix4();

	if (!result)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return result;
}

const AlignedBox Model::GetBounds() const
{
	AlignedBox result, subBound;

	for (size_t index = 0; index < this->Meshes.size(); ++index)
	{
		subBound = this->Meshes[index].GetBounds();
		result.Envelope(subBound.Minimum);
		result.Envelope(subBound.Maximum);
	}

	return result;
}

std::map<std::string, Material> Model::ParseMaterialFile(const std::string& materialPath, const std::string& materialFile)
{
	std::ifstream inputFile;
	std::stringstream inputBuffer, lineBuffer;
	std::map<std::string, Material> materialMap;
	std::string inputLine, inputToken;
	std::map<std::string, Material>::iterator materialIterator = materialMap.end();

	inputFile.open(materialPath + materialFile);

	if (!inputFile)
		return materialMap;

	inputBuffer << inputFile.rdbuf();
	inputFile.close();

	while (std::getline(inputBuffer, inputLine))
	{
		if (inputLine.empty())
			continue;

		lineBuffer = std::stringstream();
		lineBuffer << inputLine;
		lineBuffer >> inputToken;

		if (inputToken == "newmtl")
		{
			lineBuffer >> inputToken;
			materialIterator = materialMap.find(inputToken);

			if (materialIterator != materialMap.end())
			{
				materialMap.clear();
				return materialMap;
			}

			materialIterator = materialMap.insert(materialMap.begin(), std::make_pair(inputToken, Material()));
		}

		else if (inputToken == "Ns")
			lineBuffer >> materialIterator->second.Shininess;

		else if (inputToken == "Ka")
			lineBuffer >> materialIterator->second.Ambient[0] >> materialIterator->second.Ambient[1] >> materialIterator->second.Ambient[2];

		else if (inputToken == "Kd")
			lineBuffer >> materialIterator->second.Diffuse[0] >> materialIterator->second.Diffuse[1] >> materialIterator->second.Diffuse[2];

		else if (inputToken == "Ks")
			lineBuffer >> materialIterator->second.Specular[0] >> materialIterator->second.Specular[1] >> materialIterator->second.Specular[2];

		else if (inputToken == "map_Kd")
		{
			lineBuffer >> materialIterator->second.DiffuseTexture;
			materialIterator->second.DiffuseTexture = materialPath + materialIterator->second.DiffuseTexture;
		}

		else if (inputToken == "map_Ks")
		{
			lineBuffer >> materialIterator->second.SpecularTexture;
			materialIterator->second.SpecularTexture = materialPath + materialIterator->second.SpecularTexture;
		}

		else if (!lineBuffer.fail())
			continue;

		if (!lineBuffer.eof() || lineBuffer.fail())
		{
			materialMap.clear();
			return materialMap;
		}
	}

	return materialMap;
}