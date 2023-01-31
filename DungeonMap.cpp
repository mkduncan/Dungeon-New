#include "Camera.hpp"
#include "DungeonData.hpp"
#include "DungeonMap.hpp"
#include "MathManager.hpp"
#include "ShaderData.hpp"
#include "TraceManager.hpp"

#include <cmath>

std::vector<std::vector<uint8_t>> DungeonMap::Map;
float DungeonMap::MeshScale = 2.5f, DungeonMap::MeshHeight = 2.25f, DungeonMap::TextureScale = 0.25f;
bool DungeonMap::InitializeCamera = true;

static size_t FindMaxRectangle(std::vector<std::vector<uint8_t>>&, size_t&, size_t&, size_t&, size_t&);
static void EraseMaxRectangle(std::vector<std::vector<uint8_t>>&, const size_t, const size_t, const size_t, const size_t);
static bool IsUsed(const uint8_t);
static bool IsCollidable(const uint8_t);

bool DungeonMap::GenerateModel(const size_t sizeX, const size_t sizeY, Model& model, std::vector<AlignedBox>& boxes)
{
	MeshRenderable floorMesh, wallMesh;
	std::vector<std::vector<uint8_t>> cells;
	size_t maxArea = 1, indexX = 0, indexY = 0, countX = 0, countY = 0, tileCount = 0;
	float xAlpha = 0.0f, xBeta = 0.0f, zAlpha = 0.0f, zBeta = 0.0f;

	DungeonMap::Map.clear();

	if (!model.Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!DungeonData::GenerateMap(sizeX, sizeY, cells, tileCount))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	DungeonMap::Map = cells;

	while (tileCount > 0 && maxArea != 0 && tileCount <= (sizeX + 2) * (sizeY + 2))
	{
		maxArea = FindMaxRectangle(cells, indexX, indexY, countX, countY);
		tileCount -= maxArea;

		EraseMaxRectangle(cells, indexX, indexY, countX, countY);
		boxes.emplace_back();
		boxes.back().Envelope(Vector3(static_cast<float>(indexX), -DungeonMap::MeshHeight, static_cast<float>(indexY)));
		boxes.back().Envelope(Vector3(static_cast<float>(indexX + countX), DungeonMap::MeshHeight, static_cast<float>(indexY + countY)));
	}

	cells.clear();

	if (tileCount != 0)
	{
		boxes.clear();
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);
	}

	xBeta = static_cast<float>(sizeX + 1) * DungeonMap::MeshScale;
	zBeta = static_cast<float>(sizeY + 1) * DungeonMap::MeshScale;

	floorMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zAlpha), Vector3::Down, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zAlpha), Vector3::Down, Vector2(xBeta, 0.0f) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zBeta), Vector3::Down, Vector2(xBeta, zBeta) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zAlpha), Vector3::Down, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zBeta), Vector3::Down, Vector2(xBeta, zBeta) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zBeta), Vector3::Down, Vector2(0.0f, zBeta) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zAlpha), Vector3::Up, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zBeta), Vector3::Up, Vector2(xBeta, zBeta) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zAlpha), Vector3::Up, Vector2(xBeta, 0.0f) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zAlpha), Vector3::Up, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zBeta), Vector3::Up, Vector2(0.0f, zBeta) * DungeonMap::TextureScale));
	floorMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zBeta), Vector3::Up, Vector2(xBeta, zBeta) * DungeonMap::TextureScale));

	for (size_t index = 0; index < boxes.size(); ++index)
	{
		xAlpha = boxes[index].Minimum[2] * DungeonMap::MeshScale;
		xBeta = boxes[index].Maximum[2] * DungeonMap::MeshScale;
		zAlpha = boxes[index].Minimum[0] * DungeonMap::MeshScale;
		zBeta = boxes[index].Maximum[0] * DungeonMap::MeshScale;

		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zAlpha), Vector3::Right, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zBeta), Vector3::Right, Vector2(zBeta - zAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zBeta), Vector3::Right, Vector2(zBeta - zAlpha, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zAlpha), Vector3::Right, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zAlpha), Vector3::Right, Vector2(0.0f, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zBeta), Vector3::Right, Vector2(zBeta - zAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zBeta), Vector3::Out, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zBeta), Vector3::Out, Vector2(xBeta - xAlpha, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zBeta), Vector3::Out, Vector2(xBeta - xAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zBeta), Vector3::Out, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zBeta), Vector3::Out, Vector2(xBeta - xAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zBeta), Vector3::Out, Vector2(0.0f, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zAlpha), Vector3::Left, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zBeta), Vector3::Left, Vector2(zBeta - zAlpha, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zBeta), Vector3::Left, Vector2(zBeta - zAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zAlpha), Vector3::Left, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zBeta), Vector3::Left, Vector2(zBeta - zAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zAlpha), Vector3::Left, Vector2(0.0f, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zAlpha), Vector3::In, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zAlpha), Vector3::In, Vector2(xBeta - xAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, -DungeonMap::MeshHeight, zAlpha), Vector3::In, Vector2(xBeta - xAlpha, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, -DungeonMap::MeshHeight, zAlpha), Vector3::In, Vector2(0.0f, 0.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xAlpha, DungeonMap::MeshHeight, zAlpha), Vector3::In, Vector2(0.0f, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));
		wallMesh.AddVertex(VertexTextured(Vector3(xBeta, DungeonMap::MeshHeight, zAlpha), Vector3::In, Vector2(xBeta - xAlpha, DungeonMap::MeshHeight * 2.0f) * DungeonMap::TextureScale));

		boxes[index].Minimum[0] = zAlpha;
		boxes[index].Minimum[2] = xAlpha;
		boxes[index].Maximum[0] = zBeta;
		boxes[index].Maximum[2] = xBeta;
		boxes[index].Id = index * 24;
	}

	if (!floorMesh.LoadMaterial(Material(Vector3(1.0f), Vector3(0.8f), Vector3(0.5f), 360.0f)))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!floorMesh.LoadShaders(ShaderData::VertexTextured(), ShaderData::FragmentTextured()))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!floorMesh.LoadTexture("Assets/dirt_texture.png"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!wallMesh.LoadMaterial(Material(Vector3(1.0f), Vector3(0.8f), Vector3(0.5f), 360.0f)))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!wallMesh.LoadShaders(ShaderData::VertexTextured(), ShaderData::FragmentTextured()))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!wallMesh.LoadTexture("Assets/rock_texture.png"))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	model.Add(std::move(floorMesh));
	model.Add(std::move(wallMesh));

	return true;
}

//TODO: Fix the collision reaction
bool DungeonMap::BoundCamera()
{
	static const float radius = 0.25f, cameraHeightPercent = 0.75f;
	Vector3 indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);

	if (DungeonMap::InitializeCamera)
	{
		DungeonMap::InitializeCamera = false;

		for(size_t indexX = 0; indexX < DungeonMap::Map.size(); ++indexX)
			for(size_t indexY = 0; indexY < DungeonMap::Map[indexX].size(); ++indexY)
				if (DungeonMap::Map[indexX][indexY] == DungeonObject::AscendDoor)
				{
					Camera::Position[0] = static_cast<float>(indexX) * DungeonMap::MeshScale + DungeonMap::MeshScale * 0.5f;
					Camera::Position[1] = DungeonMap::MeshHeight * cameraHeightPercent * 0.5f;
					Camera::Position[2] = static_cast<float>(indexY) * DungeonMap::MeshScale + DungeonMap::MeshScale * 0.5f;
				}
	}

	if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0] + radius)][static_cast<size_t>(indexed[2])]))
	{
		Camera::Position[0] -= Camera::Delta[0];
		Camera::Delta[0] = 0.0f;
		indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);

		if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0] + radius)][static_cast<size_t>(indexed[2])]))
		{
			Camera::Position[0] -= radius * MathManager::DeltaTime(16.0f);
			Camera::Delta[0] -= radius * MathManager::DeltaTime(16.0f);
			indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);
		}
	}

	if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0] - radius)][static_cast<size_t>(indexed[2])]))
	{
		Camera::Position[0] -= Camera::Delta[0];
		Camera::Delta[0] = 0.0f;
		indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);

		if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0] - radius)][static_cast<size_t>(indexed[2])]))
		{
			Camera::Position[0] += radius * MathManager::DeltaTime(16.0f);
			Camera::Delta[0] += radius * MathManager::DeltaTime(16.0f);
			indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);
		}
	}

	if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0])][static_cast<size_t>(indexed[2] + radius)]))
	{
		Camera::Position[2] -= Camera::Delta[2];
		Camera::Delta[2] = 0.0f;
		indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);

		if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0])][static_cast<size_t>(indexed[2] + radius)]))
		{
			Camera::Position[2] -= radius * MathManager::DeltaTime(16.0f);
			Camera::Delta[2] -= radius * MathManager::DeltaTime(16.0f);
			indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);
		}
	}

	if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0])][static_cast<size_t>(indexed[2] - radius)]))
	{
		Camera::Position[2] -= Camera::Delta[2];
		Camera::Delta[2] = 0.0f;
		indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);

		if (IsCollidable(DungeonMap::Map[static_cast<size_t>(indexed[0])][static_cast<size_t>(indexed[2] - radius)]))
		{
			Camera::Position[2] += radius * MathManager::DeltaTime(16.0f);
			Camera::Delta[2] += radius * MathManager::DeltaTime(16.0f);
			indexed = Camera::Position * (1.0f / DungeonMap::MeshScale);
		}
	}

	return true;
}

static size_t FindMaxRectangle(std::vector<std::vector<uint8_t>>& matrix, size_t& indexX, size_t& indexY, size_t& sizeX, size_t& sizeY)
{
	std::vector<size_t> leftBuffer(matrix.front().size(), 0), rightBuffer(matrix.front().size(), matrix.front().size()), topBuffer(matrix.front().size(), 0), heightBuffer(matrix.front().size(), 0);
	size_t maximalArea = 0, area = 0;

	for (size_t row = 0; row < matrix.size(); ++row)
	{
		size_t curLeft = 0, curRight = matrix.front().size();

		for (size_t column = 0; column < matrix.front().size(); ++column)
		{
			if (IsUsed(matrix[row][column]))
			{
				heightBuffer[column]++;
				topBuffer[column] = row;
			}

			else
				heightBuffer[column] = 0;
		}

		for (size_t column = 0; column < matrix.front().size(); ++column)
		{
			if (IsUsed(matrix[row][column]))
				leftBuffer[column] = std::max(leftBuffer[column], curLeft);

			else
			{
				leftBuffer[column] = 0;
				curLeft = column + 1;
			}
		}

		for (int64_t column = static_cast<int64_t>(matrix.front().size()) - 1; column >= 0; --column)
		{
			if (IsUsed(matrix[row][column]))
				rightBuffer[column] = std::min(rightBuffer[column], curRight);

			else
			{
				rightBuffer[column] = matrix.front().size();
				curRight = column;
			}
		}

		for (size_t column = 0; column < matrix.front().size(); ++column)
		{
			area = (rightBuffer[column] - leftBuffer[column]) * heightBuffer[column];

			if (area > maximalArea)
			{
				maximalArea = area;
				indexX = leftBuffer[column];

				if (topBuffer[column] >= heightBuffer[column])
					indexY = topBuffer[column] - heightBuffer[column] + 1;

				else
					indexY = 0;

				sizeX = rightBuffer[column] - leftBuffer[column];
				sizeY = heightBuffer[column];
			}
		}
	}

	return maximalArea;
}

static void EraseMaxRectangle(std::vector<std::vector<uint8_t>>& cells, const size_t indexX, const size_t indexY, const size_t sizeX, const size_t sizeY)
{
	for (size_t row = indexY; row < indexY + sizeY; ++row)
		for (size_t column = indexX; column < indexX + sizeX; ++column)
			cells[row][column] = DungeonObject::Unused;
}

static bool IsUsed(const uint8_t tile)
{
	return tile == DungeonObject::Floor || tile == DungeonObject::Wall;
}

static bool IsCollidable(const uint8_t tile)
{
	return IsUsed(tile) || tile == DungeonObject::Barrel || tile == DungeonObject::Chest;
}