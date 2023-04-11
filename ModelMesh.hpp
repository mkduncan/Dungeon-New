#ifndef ModelMesh_H
#define ModelMesh_H

#include "AlignedBox.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "MeshTextured.hpp"

class ModelMesh
{
public:
	std::vector<MeshTextured> Meshes;
	std::vector<Material> Materials;
	std::string Shader;
	glm::mat4 Transform;
	AlignedBox Bounds;

	ModelMesh();
	~ModelMesh();
	ModelMesh(const ModelMesh&);
	ModelMesh(ModelMesh&&) noexcept;
	ModelMesh& operator=(const ModelMesh&);
	ModelMesh& operator=(ModelMesh&&) noexcept;

	const bool Destroy();
	const bool Load(const std::string&);
	const bool Render(const std::vector<Light>& = std::vector<Light>()) const;
	const bool Upload(const std::vector<std::vector<VertexTextured>>&, const std::vector<std::vector<GLuint>>& = std::vector<std::vector<GLuint>>());
};

#endif