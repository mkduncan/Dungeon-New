#include "Assets.hpp"
#include "Camera.hpp"
#include "DungeonMap.hpp"
#include "DungeonModel.hpp"
#include "Logger.hpp"

#define GenerationAttempts 128ULL

float DungeonModel::MeshScale = 2.5f, DungeonModel::MeshHeight = 2.25f, DungeonModel::TextureScale = 0.25f;

static const bool OutOfBounds(const size_t indexX, const size_t indexY)
{
	if (DungeonMap::Map[indexX][indexY] != DungeonTile::Unused && DungeonMap::Map[indexX][indexY] != DungeonTile::OpenedDoor && DungeonMap::Map[indexX][indexY] != DungeonTile::Corridor)
		return true;

	return false;
}

const bool DungeonModel::Generate(const size_t sizeX, const size_t sizeY)
{
	std::vector<std::vector<VertexTextured>> vertices(2, std::vector<VertexTextured>());
	Material dirtMaterial, rockMaterial;
	float xAlpha = 0.0f, xBeta = 0.0f, zAlpha = 0.0f, zBeta = 0.0f;

	if (!Assets::Models["DungeonModel"].Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!DungeonMap::Generate(sizeX, sizeY, GenerationAttempts))
		return Logger::LogError(__FUNCTION__, __LINE__);

#if !NDEBUG
	if (!DungeonMap::WriteImage("output.png"))
		return Logger::LogError(__FUNCTION__, __LINE__);
#endif

	xBeta = static_cast<float>(sizeX + 1) * DungeonModel::MeshScale;
	zBeta = static_cast<float>(sizeY + 1) * DungeonModel::MeshScale;

	vertices[0].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(xBeta, 0.0f) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(xBeta, zBeta) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(xBeta, zBeta) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, zBeta) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(xBeta, zBeta) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(xBeta, 0.0f) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, zBeta) * DungeonModel::TextureScale));
	vertices[0].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(xBeta, zBeta) * DungeonModel::TextureScale));

	for (size_t index = 0; index < DungeonMap::Bounds.size(); ++index)
	{
		xAlpha = static_cast<float>(DungeonMap::Bounds[index].Y) * DungeonModel::MeshScale;
		xBeta = static_cast<float>(DungeonMap::Bounds[index].Y + DungeonMap::Bounds[index].Height) * DungeonModel::MeshScale;
		zAlpha = static_cast<float>(DungeonMap::Bounds[index].X) * DungeonModel::MeshScale;
		zBeta = static_cast<float>(DungeonMap::Bounds[index].X + DungeonMap::Bounds[index].Width) * DungeonModel::MeshScale;

		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zAlpha), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zBeta), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(zBeta - zAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zBeta), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(zBeta - zAlpha, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zAlpha), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zAlpha), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zBeta), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(zBeta - zAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(xBeta - xAlpha, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(xBeta - xAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(xBeta - xAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zBeta), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zAlpha), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zBeta), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(zBeta - zAlpha, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zBeta), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(zBeta - zAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zAlpha), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zBeta), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(zBeta - zAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zAlpha), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(xBeta - xAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, -DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(xBeta - xAlpha, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, -DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xAlpha, DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
		vertices[1].push_back(VertexTextured(glm::vec3(xBeta, DungeonModel::MeshHeight, zAlpha), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(xBeta - xAlpha, DungeonModel::MeshHeight * 2.0f) * DungeonModel::TextureScale));
	}

	dirtMaterial.Ambient = glm::vec3(1.0f);
	dirtMaterial.Diffuse = glm::vec3(0.8f);
	dirtMaterial.Specular = glm::vec3(0.5f);
	dirtMaterial.Shininess = 360.0f;
	rockMaterial = dirtMaterial;
	dirtMaterial.DiffuseTexture = "Assets/dirt_texture.png";
	rockMaterial.DiffuseTexture = "Assets/rock_texture.png";

	Assets::Models["DungeonModel"].Materials = std::vector<Material>({ dirtMaterial, rockMaterial });
	Assets::Models["DungeonModel"].Shader = "MeshTextured";
	Assets::Models["DungeonModel"].Transform = glm::mat4(1.0f);

	if (!Assets::Models["DungeonModel"].Upload(vertices))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool DungeonModel::CollideCamera()
{
	glm::vec3 nextCameraPosition;
	int32_t currentCameraIndexX = 0, currentCameraIndexY = 0, nextCameraIndexX = 0, nextCameraIndexY = 0;
	float radius = 0.1f * DungeonModel::MeshScale;

	nextCameraPosition = Camera::Position + Camera::Delta;
	currentCameraIndexX = static_cast<int32_t>(Camera::Position.x / DungeonModel::MeshScale);
	currentCameraIndexY = static_cast<int32_t>(Camera::Position.z / DungeonModel::MeshScale);
	nextCameraIndexX = static_cast<int32_t>(nextCameraPosition.x / DungeonModel::MeshScale);
	nextCameraIndexY = static_cast<int32_t>(nextCameraPosition.z / DungeonModel::MeshScale);

	if (nextCameraIndexX <= 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (nextCameraIndexY <= 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (nextCameraIndexX >= DungeonMap::Width - 1)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (nextCameraIndexY >= DungeonMap::Height - 1)
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (OutOfBounds(currentCameraIndexX, currentCameraIndexY))
		return Logger::LogError(__FUNCTION__, __LINE__);

	nextCameraIndexX = static_cast<int32_t>((nextCameraPosition.x + radius) / DungeonModel::MeshScale);

	if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexX + 1 == nextCameraIndexX)
		Camera::Delta.x = -0.0f;

	else
	{
		nextCameraIndexX = static_cast<int32_t>((nextCameraPosition.x - radius) / DungeonModel::MeshScale);

		if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexX - 1 == nextCameraIndexX)
			Camera::Delta.x = 0.0f;
	}

	nextCameraIndexX = static_cast<int32_t>(nextCameraPosition.x / DungeonModel::MeshScale);
	nextCameraIndexY = static_cast<int32_t>((nextCameraPosition.z + radius) / DungeonModel::MeshScale);

	if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexY + 1 == nextCameraIndexY)
		Camera::Delta.z = -0.0f;

	else
	{
		nextCameraIndexY = static_cast<int32_t>((nextCameraPosition.z - radius) / DungeonModel::MeshScale);

		if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexY - 1 == nextCameraIndexY)
			Camera::Delta.z = 0.0f;
	}

	nextCameraIndexX = static_cast<int32_t>((nextCameraPosition.x + radius) / DungeonModel::MeshScale);
	nextCameraIndexY = static_cast<int32_t>((nextCameraPosition.z + radius) / DungeonModel::MeshScale);

	if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexX + 1 == nextCameraIndexX && currentCameraIndexY + 1 == nextCameraIndexY && !OutOfBounds(nextCameraIndexX - 1, nextCameraIndexY) && !OutOfBounds(nextCameraIndexX, nextCameraIndexY - 1))
		Camera::Delta = glm::vec3(-radius, 0.0f, -radius) * 0.0625f;

	nextCameraIndexX = static_cast<int32_t>((nextCameraPosition.x - radius) / DungeonModel::MeshScale);

	if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexX - 1 == nextCameraIndexX && currentCameraIndexY + 1 == nextCameraIndexY && !OutOfBounds(nextCameraIndexX + 1, nextCameraIndexY) && !OutOfBounds(nextCameraIndexX, nextCameraIndexY - 1))
		Camera::Delta = glm::vec3(radius, 0.0f, -radius) * 0.0625f;

	nextCameraIndexY = static_cast<int32_t>((nextCameraPosition.z - radius) / DungeonModel::MeshScale);

	if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexX - 1 == nextCameraIndexX && currentCameraIndexY - 1 == nextCameraIndexY && !OutOfBounds(nextCameraIndexX + 1, nextCameraIndexY) && !OutOfBounds(nextCameraIndexX, nextCameraIndexY + 1))
		Camera::Delta = glm::vec3(radius, 0.0f, radius) * 0.0625f;

	nextCameraIndexX = static_cast<int32_t>((nextCameraPosition.x + radius) / DungeonModel::MeshScale);

	if (OutOfBounds(nextCameraIndexX, nextCameraIndexY) && currentCameraIndexX + 1 == nextCameraIndexX && currentCameraIndexY - 1 == nextCameraIndexY && !OutOfBounds(nextCameraIndexX - 1, nextCameraIndexY) && !OutOfBounds(nextCameraIndexX, nextCameraIndexY + 1))
		Camera::Delta = glm::vec3(-radius, 0.0f, radius) * 0.0625f;

	return true;
}

const bool DungeonModel::PlaceCamera()
{
	if (DungeonMap::Map.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	for (size_t indexX = 0; indexX < DungeonMap::Width; ++indexX)
		for (size_t indexY = 0; indexY < DungeonMap::Height; ++indexY)
			if (DungeonMap::Map[indexX][indexY] == DungeonTile::AscendDoor)
			{
				Camera::Position = glm::vec3((static_cast<float>(indexX) + 0.5f) * DungeonModel::MeshScale, 0.0f, (static_cast<float>(indexY - 1) + 0.5f) * DungeonModel::MeshScale);
				Camera::Yaw = 90.0f;

				return true;
			}

	return Logger::LogError(__FUNCTION__, __LINE__);
}