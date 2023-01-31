#ifndef MeshRenderable_H
#define MeshRenderable_H

#include "AlignedBox.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Matrix4.hpp"
#include "MeshColored.hpp"
#include "MeshTextured.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <glad/glad.h>
#include <map>

class MeshRenderable
{
private:
	MeshColored SubMeshColored;
	MeshTextured SubMeshTextured;
	Shader SubShader;
	Texture SubTexture;
	Material SubMaterial;

	friend class Model;

public:
	MeshRenderable();
	~MeshRenderable();
	MeshRenderable(const MeshRenderable&) = delete;
	MeshRenderable(MeshRenderable&&) noexcept;
	MeshRenderable& operator=(const MeshRenderable&) = delete;
	MeshRenderable& operator=(MeshRenderable&&) noexcept;

	bool AddVertex(const VertexColored&);
	bool AddVertex(const VertexTextured&);
	bool LoadShaders(const std::string&, const std::string&, const bool = false);
	bool LoadTexture(const std::string&);
	bool LoadMaterial(const Material&);
	bool SetIndices(const std::vector<GLuint>&);
	bool Upload();
	bool Render(const Matrix4 & = Matrix4(), const std::map<LightType, std::vector<Light>> & = std::map<LightType, std::vector<Light>>()) const;
	bool Destroy();

	const AlignedBox GetBounds() const;
};

#endif