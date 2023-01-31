#include "OrientedBox.hpp"
#include <limits>

OrientedBox::OrientedBox() : Transform(), Minimum(Vector3(std::numeric_limits<float>::max())), Maximum(Vector3(-std::numeric_limits<float>::max()))
{
}

OrientedBox::~OrientedBox()
{
}

OrientedBox::OrientedBox(const OrientedBox& box) : Transform(box.Transform), Minimum(box.Minimum), Maximum(box.Maximum)
{
}

OrientedBox::OrientedBox(OrientedBox&& box) noexcept : Transform(std::move(box.Transform)), Minimum(std::move(box.Minimum)), Maximum(std::move(box.Maximum))
{
}

OrientedBox& OrientedBox::operator=(const OrientedBox& box)
{
	this->Transform = box.Transform;
	this->Minimum = box.Minimum;
	this->Maximum = box.Maximum;

	return (*this);
}

OrientedBox& OrientedBox::operator=(OrientedBox&& box) noexcept
{
	this->Transform = std::move(box.Transform);
	this->Minimum = std::move(box.Minimum);
	this->Maximum = std::move(box.Maximum);

	return (*this);
}

const Matrix4 OrientedBox::GetTransform() const
{
	return this->Transform;
}

bool OrientedBox::SetTransform(const Matrix4& transform)
{
	this->Transform = transform;
	return true;
}

bool OrientedBox::SetBounds(const AlignedBox& box, const Matrix4& transform)
{
	this->Transform = transform;
	this->Minimum = box.Minimum;
	this->Maximum = box.Maximum;

	return true;
}

#include "Camera.hpp"
#include "MeshRenderable.hpp"
#include "Ray3.hpp"
#include "ShaderData.hpp"
#include "TraceManager.hpp"
const bool OrientedBox::Render() const
{
	MeshRenderable boxMesh;
	Vector4 boxColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	Vector3 center = (this->Minimum + this->Maximum) * 0.5f;
	float pickDistance = 0.0;

	std::vector<Vector3> pointSoup =
	{
		this->Minimum,
		Vector3(this->Maximum[0], this->Minimum[1], this->Minimum[2]),
		Vector3(this->Maximum[0], this->Maximum[1], this->Minimum[2]),
		Vector3(this->Minimum[0], this->Maximum[1], this->Minimum[2]),
		Vector3(this->Minimum[0], this->Minimum[1], this->Maximum[2]),
		Vector3(this->Maximum[0], this->Minimum[1], this->Maximum[2]),
		this->Maximum,
		Vector3(this->Minimum[0], this->Maximum[1], this->Maximum[2]),
	};

	for (size_t index = 0; index < pointSoup.size(); ++index)
		pointSoup[index] = this->Transform * (pointSoup[index] - center) + center;

	Ray3 pick(Camera::Position, Camera::Front);

	if (pick.Picks(*this, pickDistance))
		boxColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[0]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[1]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[2]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[2]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[3]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[0]));

	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[1]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[5]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[6]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[6]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[2]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[1]));

	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[4]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[5]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[6]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[6]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[7]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[4]));

	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[4]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[0]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[3]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[3]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[7]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[4]));

	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[0]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[1]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[5]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[5]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[4]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[0]));

	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[3]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[2]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[6]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[6]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[7]));
	boxMesh.AddVertex(VertexColored(boxColor, pointSoup[3]));

	if (!boxMesh.LoadShaders(ShaderData::VertexColored(), ShaderData::FragmentColored()))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!boxMesh.Upload())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (!boxMesh.Render())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	if (!boxMesh.Destroy())
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}