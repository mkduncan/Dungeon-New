#ifndef Model_H
#define Model_H

#include "AlignedBox.hpp"
#include "Matrix4.hpp"
#include "MeshRenderable.hpp"

#include <map>
#include <vector>

class Model
{
private:
	std::vector<MeshRenderable> Meshes;
	Matrix4 Transform;

	static std::map<std::string, Material> ParseMaterialFile(const std::string&, const std::string&);

public:
	Model();
	~Model();
	Model(const Model&) = delete;
	Model(Model&&) noexcept;
	Model(MeshRenderable&&) noexcept;
	Model& operator=(const Model&) = delete;
	Model& operator=(Model&&) noexcept;
	Model& operator=(MeshRenderable&&) noexcept;

	const Matrix4& GetTransform() const;
	bool SetTransform(const Matrix4&);

	bool Add(MeshRenderable&&);
	bool Load(const std::string&);
	bool Upload();
	bool Render(const std::map<LightType, std::vector<Light>> & = std::map<LightType, std::vector<Light>>()) const;
	bool Destroy();

	const AlignedBox GetBounds() const;
};

#endif