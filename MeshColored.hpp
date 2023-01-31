#ifndef MeshColored_H
#define MeshColored_H

#include "VertexColored.hpp"

#include <glad/glad.h>
#include <vector>

class MeshColored
{
private:
	std::vector<VertexColored> Vertices;
	std::vector<GLuint> Indices;
	GLuint ArrayId, BufferId, IndexId;

	friend class MeshRenderable;

public:
	MeshColored();
	~MeshColored();
	MeshColored(const MeshColored&) = delete;
	MeshColored(MeshColored&&) noexcept;
	MeshColored& operator=(const MeshColored&) = delete;
	MeshColored& operator=(MeshColored&&) noexcept;

	bool AddVertex(const VertexColored&);
	bool SetIndices(const std::vector<GLuint>&);
	bool Upload();
	bool Render() const;
	bool Empty() const;
	bool Destroy();
};

#endif