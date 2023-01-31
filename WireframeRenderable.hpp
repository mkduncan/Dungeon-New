#ifndef WireframeRenderable
#define WireframeRenderable

#include "Shader.hpp"
#include "VertexColored.hpp"
#include <vector>

class WireFrameRenderable
{
public:
	Shader SubShader;
	std::vector<VertexColored> Vertices;
	std::vector<GLuint> Indices;
	GLuint ArrayId, BufferId, IndexId;

	WireFrameRenderable();
	~WireFrameRenderable();
	WireFrameRenderable(const WireFrameRenderable&) = delete;
	WireFrameRenderable(WireFrameRenderable&&) noexcept;
	WireFrameRenderable& operator=(const WireFrameRenderable&) = delete;
	WireFrameRenderable& operator=(WireFrameRenderable&&) noexcept;

	bool AddVertex(const VertexColored&);
	bool LoadShaders(const std::string&, const std::string&, const bool = false);
	bool SetIndices(const std::vector<GLuint>&);
	bool Upload();
	bool Render(const Matrix4& = Matrix4()) const;
	bool Destroy();
};

#endif