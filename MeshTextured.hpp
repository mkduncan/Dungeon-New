#ifndef MeshTextured_H
#define MeshTextured_H

#include "VertexTextured.hpp"

#include <glad/glad.h>
#include <vector>

class MeshTextured
{
private:
	std::vector<VertexTextured> Vertices;
	std::vector<GLuint> Indices;
	GLuint ArrayId, BufferId, IndexId;

	friend class MeshRenderable;

public:
	MeshTextured();
	~MeshTextured();
	MeshTextured(const MeshTextured&) = delete;
	MeshTextured(MeshTextured&&) noexcept;
	MeshTextured& operator=(const MeshTextured&) = delete;
	MeshTextured& operator=(MeshTextured&&) noexcept;

	bool AddVertex(const VertexTextured&);
	bool SetIndices(const std::vector<GLuint>&);
	bool Upload();
	bool Render() const;
	bool Empty() const;
	bool Destroy();
};

#endif