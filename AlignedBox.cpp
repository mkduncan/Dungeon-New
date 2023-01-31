#include "AlignedBox.hpp"
#include "MathManager.hpp"
#include <limits>

AlignedBox::AlignedBox() : Minimum(Vector3(std::numeric_limits<float>::max())), Maximum(Vector3(-std::numeric_limits<float>::max())), Id(0)
{
}

AlignedBox::~AlignedBox()
{
}

AlignedBox::AlignedBox(const AlignedBox& box) : Minimum(box.Minimum), Maximum(box.Maximum), Id(box.Id)
{
}

AlignedBox::AlignedBox(AlignedBox&& box) noexcept : Minimum(std::move(box.Minimum)), Maximum(std::move(box.Maximum)), Id(MathManager::MoveScalar(box.Id))
{
}

AlignedBox& AlignedBox::operator=(const AlignedBox& box)
{
	this->Minimum = box.Minimum;
	this->Maximum = box.Maximum;
	this->Id = box.Id;

	return *this;
}

AlignedBox& AlignedBox::operator=(AlignedBox&& box) noexcept
{
	this->Minimum = std::move(box.Minimum);
	this->Maximum = std::move(box.Maximum);
	this->Id = MathManager::MoveScalar(box.Id);

	return *this;
}

bool AlignedBox::Envelope(const Vector3& point)
{
	this->Minimum = Vector3(std::fminf(this->Minimum[0], point[0]), std::fminf(this->Minimum[1], point[1]), std::fminf(this->Minimum[2], point[2]));
	this->Maximum = Vector3(std::fmaxf(this->Maximum[0], point[0]), std::fmaxf(this->Maximum[1], point[1]), std::fmaxf(this->Maximum[2], point[2]));

	return true;
}

bool AlignedBox::Overlaps(const AlignedBox& box) const
{
	return this->Maximum[0] > box.Minimum[0] && this->Minimum[0] < box.Maximum[0] && this->Maximum[1] > box.Minimum[1] && this->Minimum[1] < box.Maximum[1] && this->Maximum[2] > box.Minimum[2] && this->Minimum[2] < box.Maximum[2];
}

const std::array<Vector3, 8> AlignedBox::Corners() const
{
	return std::array<Vector3, 8>
	({
		this->Minimum,
		Vector3(this->Maximum[0], this->Minimum[1], this->Minimum[2]),
		Vector3(this->Maximum[0], this->Maximum[1], this->Minimum[2]),
		Vector3(this->Minimum[0], this->Maximum[1], this->Minimum[2]),
		Vector3(this->Minimum[0], this->Minimum[1], this->Maximum[2]),
		Vector3(this->Maximum[0], this->Minimum[1], this->Maximum[2]),
		this->Maximum,
		Vector3(this->Minimum[0], this->Maximum[1], this->Maximum[2])
	});
}

const Vector3 AlignedBox::Center() const
{
	return (Minimum + Maximum) * 0.5f;
}

const Vector3 AlignedBox::Extent() const
{
	return (Maximum - Minimum) * 0.5f;
}

#include "MeshRenderable.hpp"
#include "ShaderData.hpp"
#include "TraceManager.hpp"
const bool AlignedBox::Render() const
{
	MeshRenderable boxMesh;
	Vector4 boxColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

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