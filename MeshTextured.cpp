#include "Logger.hpp"
#include "MeshTextured.hpp"

MeshTextured::MeshTextured() : ArrayId(0), BufferId(0), IndexId(0), Indices(0)
{
}

MeshTextured::~MeshTextured()
{
	if (!this->Destroy())
		Logger::LogError(__FUNCTION__, __LINE__);
}

MeshTextured::MeshTextured(const MeshTextured& mesh) : ArrayId(mesh.ArrayId), BufferId(mesh.BufferId), IndexId(mesh.IndexId), Indices(mesh.Indices)
{
}

MeshTextured::MeshTextured(MeshTextured&& mesh) noexcept : ArrayId(mesh.ArrayId), BufferId(mesh.BufferId), IndexId(mesh.IndexId), Indices(mesh.Indices)
{
	mesh.ArrayId = 0;
	mesh.BufferId = 0;
	mesh.IndexId = 0;
	mesh.Indices = 0;
}

MeshTextured& MeshTextured::operator=(const MeshTextured& mesh)
{
	this->ArrayId = mesh.ArrayId;
	this->BufferId = mesh.BufferId;
	this->IndexId = mesh.IndexId;
	this->Indices = mesh.Indices;

	return (*this);
}

MeshTextured& MeshTextured::operator=(MeshTextured&& mesh) noexcept
{
	if (this->ArrayId != mesh.ArrayId && this->BufferId != mesh.BufferId && this->IndexId != mesh.IndexId)
	{
		if (!this->Destroy())
			Logger::LogError(__FUNCTION__, __LINE__);

		this->ArrayId = mesh.ArrayId;
		this->BufferId = mesh.BufferId;
		this->IndexId = mesh.IndexId;
		this->Indices = mesh.Indices;

		mesh.ArrayId = 0;
		mesh.BufferId = 0;
		mesh.IndexId = 0;
		mesh.Indices = 0;
	}

	return (*this);
}

const bool MeshTextured::Destroy()
{
	bool result = true;

	if (this->ArrayId != 0)
	{
		glDeleteVertexArrays(1, &this->ArrayId);
		this->ArrayId = 0;

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
			result = false;
	}

	if (this->BufferId != 0)
	{
		glDeleteBuffers(1, &this->BufferId);
		this->BufferId = 0;

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
			result = false;
	}

	if (this->IndexId != 0)
	{
		glDeleteBuffers(1, &this->IndexId);
		this->IndexId = 0;

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
			result = false;
	}

	this->Indices = 0;
	return result;
}

const bool MeshTextured::Render() const
{
	if (this->ArrayId == 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (this->BufferId == 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (this->IndexId == 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (this->Indices == 0)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glBindVertexArray(this->ArrayId);
	glDrawElements(GL_TRIANGLES, this->Indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool MeshTextured::Upload(const std::vector<VertexTextured>& vertices, const std::vector<GLuint>& indices)
{
	std::vector<GLuint> indexArray;

	if (!this->Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (vertices.empty())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (indices.empty())
	{
		indexArray.resize(vertices.size());

		for (size_t index = 0; index < indexArray.size(); ++index)
			indexArray[index] = static_cast<GLuint>(index);

		glGenVertexArrays(1, &this->ArrayId);
		glGenBuffers(1, &this->BufferId);
		glGenBuffers(1, &this->IndexId);
		glBindVertexArray(this->ArrayId);
		glBindBuffer(GL_ARRAY_BUFFER, this->BufferId);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(VertexTextured)), &(vertices[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IndexId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexArray.size() * sizeof(GLuint)), &(indexArray[0]), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*)offsetof(VertexTextured, Position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*)offsetof(VertexTextured, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*)offsetof(VertexTextured, Texture));
		glBindVertexArray(0);

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
		{
			this->Destroy();
			return false;
		}

		this->Indices = static_cast<GLsizei>(indexArray.size());
	}

	else
	{
		glGenVertexArrays(1, &this->ArrayId);
		glGenBuffers(1, &this->BufferId);
		glGenBuffers(1, &this->IndexId);
		glBindVertexArray(this->ArrayId);
		glBindBuffer(GL_ARRAY_BUFFER, this->BufferId);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(VertexTextured)), &(vertices[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IndexId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)), &(indices[0]), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*)offsetof(VertexTextured, Position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*)offsetof(VertexTextured, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*)offsetof(VertexTextured, Texture));
		glBindVertexArray(0);

		if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
		{
			this->Destroy();
			return false;
		}

		this->Indices = static_cast<GLsizei>(indices.size());
	}

	return true;
}