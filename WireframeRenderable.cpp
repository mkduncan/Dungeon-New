#include "MathManager.hpp"
#include "TraceManager.hpp"
#include "WireframeRenderable.hpp"

Shader SubShader;
std::vector<VertexColored> Vertices;
std::vector<GLuint> Indices;
GLuint ArrayId, BufferId, IndexId;

WireFrameRenderable::WireFrameRenderable() : SubShader(), Vertices(), Indices(), ArrayId(0), BufferId(0), IndexId(0)
{
}

WireFrameRenderable::~WireFrameRenderable()
{
}

WireFrameRenderable::WireFrameRenderable(WireFrameRenderable&& renderable) noexcept : SubShader(std::move(renderable.SubShader)), Vertices(std::move(renderable.Vertices)), Indices(std::move(renderable.Indices)), ArrayId(MathManager::MoveScalar(renderable.ArrayId)), BufferId(MathManager::MoveScalar(renderable.BufferId)), IndexId(MathManager::MoveScalar(renderable.IndexId))
{
}

WireFrameRenderable& WireFrameRenderable::operator=(WireFrameRenderable&& renderable) noexcept
{
	this->SubShader = std::move(renderable.SubShader);
	this->Vertices = std::move(renderable.Vertices);
	this->Indices = std::move(renderable.Indices);
	this->ArrayId = MathManager::MoveScalar(renderable.ArrayId);
	this->BufferId = MathManager::MoveScalar(renderable.BufferId);
	this->IndexId = MathManager::MoveScalar(renderable.IndexId);

	return (*this);
}

bool WireFrameRenderable::AddVertex(const VertexColored& vertex)
{
	this->Vertices.push_back(vertex);
	return true;
}

bool WireFrameRenderable::LoadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const bool loadFromFile)
{
	if (!this->SubShader.Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (loadFromFile && !this->SubShader.Load(vertexShaderPath, fragmentShaderPath))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!loadFromFile && !this->SubShader.Create(vertexShaderPath, fragmentShaderPath))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}

bool WireFrameRenderable::SetIndices(const std::vector<GLuint>& indices)
{
	this->Indices = indices;
	return true;
}

bool WireFrameRenderable::Upload()
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
		this->Indices.resize(this->Vertices.size() << 1);

		for (size_t index = 1; index < this->Vertices.size(); ++index)
		{
			this->Indices[index - 1] = static_cast<GLuint>(index - 1);
			this->Indices[index] = static_cast<GLuint>(index);
		}
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

bool WireFrameRenderable::Render(const Matrix4& model) const
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
	glDrawElements(GL_LINES, static_cast<GLsizei>(this->Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool WireFrameRenderable::Destroy()
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