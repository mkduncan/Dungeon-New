#ifndef Camera_H
#define Camera_H

#include "Matrix4.hpp"
#include "Shader.hpp"
#include "Vector3.hpp"

#include <cstdint>

class Camera
{
public:
	static Matrix4 Projection, View;
	static Vector3 Position, Delta, Right, Up, Front;
	static double Mouse[2];
	static float Pitch, Yaw;
	static bool Click[2];

	Camera() = delete;
	~Camera() = delete;
	Camera(const Camera&) = delete;
	Camera(Camera&&) noexcept = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) noexcept = delete;

	friend class Scene;
	friend class DungeonManager;
	friend class DungeonMap;

public:
	static bool Update(const bool = true, const bool = true);
	static bool LoadWorldMatrix(const Shader&, const Matrix4 & = Matrix4());
};

#endif