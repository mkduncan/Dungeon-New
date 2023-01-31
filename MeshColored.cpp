#include "MathManager.hpp"
#include "MeshColored.hpp"
#include "TraceManager.hpp"

MeshColored::MeshColored() : Vertices(), Indices(), ArrayId(0), BufferId(0), IndexId(0)
{
}

MeshColored::~MeshColored()
{
	if (!this->Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
}

MeshColored::MeshColored(MeshColored&& mesh) noexcept : Vertices(std::move(mesh.Vertices)), Indices(std::move(mesh.Indices)), ArrayId(MathManager::MoveScalar(mesh.ArrayId)), BufferId(MathManager::MoveScalar(mesh.BufferId)), IndexId(MathManager::MoveScalar(mesh.IndexId))
{
}

MeshColored& MeshColored::operator=(MeshColored&& mesh) noexcept
{
	this->Vertices = std::move(mesh.Vertices);
	this->Indices = std::move(mesh.Indices);
	this->ArrayId = MathManager::MoveScalar(mesh.ArrayId);
	this->BufferId = MathManager::MoveScalar(mesh.BufferId);
	this->IndexId = MathManager::MoveScalar(mesh.IndexId);

	return (*this);
}

bool MeshColored::AddVertex(const VertexColored& vertex)
{
	this->Vertices.push_back(vertex);
	return true;
}

bool MeshColored::SetIndices(const std::vector<GLuint>& indices)
{
	this->Indices = indices;
	return true;
}

bool MeshColored::Upload()
{
	if (this->Vertices.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (this->ArrayId != 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (this->BufferId != 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (this->IndexId != 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	if (this->Indices.empty())
	{
		this->Indices.resize(this->Vertices.size());

		for (size_t index = 0; index < this->Indices.size(); ++index)
			this->Indices[index] = static_cast<GLuint>(index);
	}

	glGenVertexArrays(1, &this->ArrayId);
	glGenBuffers(1, &this->BufferId);
	glGenBuffers(1, &this->IndexId);
	glBindVertexArray(this->ArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, this->BufferId);
	glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(VertexColored), &this->Vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IndexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(GLuint), &this->Indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColored), (void*)offsetof(VertexColored, Position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexColored), (void*)offsetof(VertexColored, Color));
	glBindVertexArray(0);

	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool MeshColored::Render() const
{
	if (this->Indices.empty())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (this->ArrayId == 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (this->BufferId == 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (this->IndexId == 0)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glBindVertexArray(this->ArrayId);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->Indices.size() * sizeof(GLuint), &this->Indices[0]);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool MeshColored::Empty() const
{
	return this->Vertices.empty();
}

bool MeshColored::Destroy()
{
	bool result = true;

	this->Vertices.clear();
	this->Indices.clear();

	if (this->ArrayId != 0)
	{
		glDeleteVertexArrays(1, &this->ArrayId);
		this->ArrayId = 0;

		if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
			result = false;
	}

	if (this->BufferId != 0)
	{
		glDeleteBuffers(1, &this->BufferId);
		this->BufferId = 0;

		if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
			result = false;
	}

	if (this->IndexId != 0)
	{
		glDeleteBuffers(1, &this->IndexId);
		this->IndexId = 0;

		if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
			result = false;
	}

	return result;
}