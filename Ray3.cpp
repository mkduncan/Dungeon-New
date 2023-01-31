#include "Ray3.hpp"
#include <cmath>

Ray3::Ray3() : Position(), Direction()
{
}

Ray3::~Ray3()
{
}

Ray3::Ray3(const Ray3& ray) : Position(ray.Position), Direction(ray.Direction)
{
}

Ray3::Ray3(Ray3&& ray) noexcept : Position(std::move(ray.Position)), Direction(std::move(ray.Direction))
{
}

Ray3::Ray3(const Vector3& position, const Vector3& direction) : Position(position), Direction(direction)
{
}

Ray3& Ray3::operator=(const Ray3& ray)
{
	this->Position = ray.Position;
	this->Direction = ray.Direction;

	return (*this);
}

Ray3& Ray3::operator=(Ray3&& ray) noexcept
{
	this->Position = std::move(ray.Position);
	this->Direction = std::move(ray.Direction);

	return (*this);
}

bool Ray3::Picks(const OrientedBox& box, float& distance) const
{
	Matrix4 boxInverse(Matrix4::Inverse(Matrix4(box.Transform[0], box.Transform[1], box.Transform[2], Vector4(0.0f, 0.0f, 0.0f, 1.0f))));
	Vector3 center = (box.Minimum + box.Maximum) * 0.5f;

	Vector3 position = boxInverse * (this->Position - center) + center;
	Vector3 direction = boxInverse * (this->Position + this->Direction - center) + center - position;
	Vector3 dirfrac;

	direction = direction * (1.0f / std::sqrtf(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]));

	//
	dirfrac[0] = 1.0f / direction[0];
	dirfrac[1] = 1.0f / direction[1];
	dirfrac[2] = 1.0f / direction[2];

	float t1 = (box.Minimum[0] - position[0]) * dirfrac[0];
	float t2 = (box.Maximum[0] - position[0]) * dirfrac[0];
	float t3 = (box.Minimum[1] - position[1]) * dirfrac[1];
	float t4 = (box.Maximum[1] - position[1]) * dirfrac[1];
	float t5 = (box.Minimum[2] - position[2]) * dirfrac[2];
	float t6 = (box.Maximum[2] - position[2]) * dirfrac[2];

	float tmin = std::fmaxf(std::fmaxf(std::fminf(t1, t2), std::fminf(t3, t4)), std::fminf(t5, t6));
	float tmax = std::fminf(std::fminf(std::fmaxf(t1, t2), std::fmaxf(t3, t4)), std::fmaxf(t5, t6));

	if (tmax < 0)
	{
		distance = std::numeric_limits<float>::max();
		return false;
	}

	if (tmin > tmax)
	{
		distance = std::numeric_limits<float>::max();
		return false;
	}

	distance = tmin;
	return true;
	//
}