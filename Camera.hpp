#ifndef Camera_H
#define Camera_H

#include "Shader.hpp"

class Camera
{
public:
	static glm::mat4 Projection, View;
	static glm::vec3 Position, Delta, Right, Up, Front;
	static float Pitch, Yaw, MovementSpeed, LookSpeed;

	Camera() = delete;
	~Camera() = delete;
	Camera(const Camera&) = delete;
	Camera(Camera&&) noexcept = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) noexcept = delete;

	static const bool Enable(const Shader&, const glm::mat4& = glm::mat4(1.0f));
	static const bool Update(const bool = true, const bool = true, const bool = true);
};

#endif