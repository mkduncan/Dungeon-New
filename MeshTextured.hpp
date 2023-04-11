#ifndef MeshTextured_H
#define MeshTextured_H

#include "VertexTextured.hpp"

#include <glad/glad.h>
#include <vector>

class MeshTextured
{
private:
	GLuint ArrayId, BufferId, IndexId;
	GLsizei Indices;

public:
	MeshTextured();
	~MeshTextured();
	MeshTextured(const MeshTextured&);
	MeshTextured(MeshTextured&&) noexcept;
	MeshTextured& operator=(const MeshTextured&);
	MeshTextured& operator=(MeshTextured&&) noexcept;

	const bool Destroy();
	const bool Render() const;
	const bool Upload(const std::vector<VertexTextured>&, const std::vector<GLuint>& = std::vector<GLuint>());
};

#endif