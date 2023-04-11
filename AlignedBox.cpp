#include "AlignedBox.hpp"
#include "Assets.hpp"
#include "Camera.hpp"
#include "Logger.hpp"
#include "MeshColored.hpp"

#include <limits>

AlignedBox::AlignedBox() : Minimum(std::numeric_limits<float>::max()), Maximum(-std::numeric_limits<float>::max())
{
}

AlignedBox::~AlignedBox()
{
}

AlignedBox::AlignedBox(const glm::vec3& minimum, const glm::vec3& maximum) : Minimum(minimum), Maximum(maximum)
{
}

AlignedBox::AlignedBox(const AlignedBox& box) : Minimum(box.Minimum), Maximum(box.Maximum)
{
}

AlignedBox::AlignedBox(AlignedBox&& box) noexcept : Minimum(box.Minimum), Maximum(box.Maximum)
{
	box.Minimum = glm::vec3(std::numeric_limits<float>::max());
	box.Maximum = glm::vec3(-std::numeric_limits<float>::max());
}

AlignedBox& AlignedBox::operator=(const AlignedBox& box)
{
	this->Minimum = box.Minimum;
	this->Maximum = box.Maximum;

	return (*this);
}

AlignedBox& AlignedBox::operator=(AlignedBox&& box) noexcept
{
	this->Minimum = box.Minimum;
	this->Maximum = box.Maximum;
	box.Minimum = glm::vec3(std::numeric_limits<float>::max());
	box.Maximum = glm::vec3(-std::numeric_limits<float>::max());

	return (*this);
}

const bool AlignedBox::Expand(const glm::vec3& point)
{
	this->Minimum = glm::min(this->Minimum, point);
	this->Maximum = glm::max(this->Maximum, point);

	return true;
}

//	TODO: Remove this debug code.
const bool AlignedBox::Render(const glm::mat4& transform, const glm::vec4& color) const
{
	MeshColored mesh;
	std::vector<VertexColored> vertices(8);
	std::vector<glm::vec3> points;
	std::vector<GLuint> indices;
	glm::vec3 center = (this->Minimum + this->Maximum) * 0.5f;

	points = std::vector<glm::vec3>
	{
		this->Minimum,
		glm::vec3(this->Minimum.x, this->Minimum.y, this->Maximum.z),
		glm::vec3(this->Minimum.x, this->Maximum.y, this->Minimum.z),
		glm::vec3(this->Minimum.x, this->Maximum.y, this->Maximum.z),
		glm::vec3(this->Maximum.x, this->Minimum.y, this->Minimum.z),
		glm::vec3(this->Maximum.x, this->Minimum.y, this->Maximum.z),
		glm::vec3(this->Maximum.x, this->Maximum.y, this->Minimum.z),
		this->Maximum
	};

	indices = std::vector<GLuint>
	{
		0, 2, 3,
		0, 3, 1,
		2, 6, 7,
		2, 7, 3,
		6, 4, 5,
		6, 5, 7,
		4, 0, 1,
		4, 1, 5,
		0, 4, 6,
		0, 6, 2,
		1, 5, 7,
		1, 7, 3,
		5, 4, 2
	};

	for (size_t index = 0; index < vertices.size(); ++index)
		vertices[index] = VertexColored(points[index], color);

	if (!mesh.Upload(vertices, indices))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!Assets::Shaders["MeshColored"].Enable())
	{
		mesh.Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	else if (!Camera::Enable(Assets::Shaders["MeshColored"], transform))
	{
		mesh.Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	else if (!mesh.Render())
	{
		mesh.Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	mesh.Destroy();
	return true;
}