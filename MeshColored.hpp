#ifndef MeshColored_H
#define MeshColored_H

#include "VertexColored.hpp"

#include <glad/glad.h>
#include <vector>

class MeshColored
{
private:
	GLuint ArrayId, BufferId, IndexId;
	GLsizei Indices;

public:
	MeshColored();
	~MeshColored();
	MeshColored(const MeshColored&);
	MeshColored(MeshColored&&) noexcept;
	MeshColored& operator=(const MeshColored&);
	MeshColored& operator=(MeshColored&&) noexcept;

	const bool Destroy();
	const bool Render() const;
	const bool Upload(const std::vector<VertexColored>&, const std::vector<GLuint>& = std::vector<GLuint>());
};

#endif 