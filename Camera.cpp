#include "Camera.hpp"
#include "FrameRenderable.hpp"
#include "MathManager.hpp"
#include "TraceManager.hpp"

#include <cmath>

#define MOVEMENT_SPEED 6.0f
#define SPEED_MULTIPLIER 4.0f
#define LOOK_SPEED 36.0f

Matrix4 Camera::Projection(Matrix4::Perspective(MathManager::Radians(90.0f), 1.0f, 0.1f, 32.0f)), Camera::View(Matrix4::LookAt(Vector3(), Vector3::In, Vector3::Up));
Vector3 Camera::Position, Camera::Delta, Camera::Right(Vector3::Right), Camera::Up(Vector3::Up), Camera::Front(Vector3::In);
double Camera::Mouse[2] = { -1.0, -1.0 };
float Camera::Pitch = 0.0f, Camera::Yaw = -90.0f;
bool Camera::Click[2] = { false, false };

bool Camera::Update(const bool lockY, const bool lockSpeed)
{
	double newMouse[2] = { Camera::Mouse[0], Camera::Mouse[1] };
	float movementScalar = 1.0f;

	if (FrameRenderable::Interface == nullptr)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	if (Camera::Mouse[0] < 0.0 && Camera::Mouse[1] < 0.0)
	{
		glfwGetCursorPos(FrameRenderable::Interface, &Camera::Mouse[0], &Camera::Mouse[1]);
		Camera::Click[0] = (glfwGetMouseButton(FrameRenderable::Interface, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

		return true;
	}

	Camera::Click[1] = Camera::Click[0];
	glfwGetCursorPos(FrameRenderable::Interface, &Camera::Mouse[0], &Camera::Mouse[1]);
	Camera::Click[0] = (glfwGetMouseButton(FrameRenderable::Interface, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	Camera::Delta = Vector3();

	if (!lockSpeed && (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
		movementScalar = SPEED_MULTIPLIER;

	if (lockY)
	{
		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_LEFT) == GLFW_PRESS)
			Camera::Delta -= Vector3::Normalize(Vector3(Camera::Right[0], 0.0f, Camera::Right[2])) * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);

		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_RIGHT) == GLFW_PRESS)
			Camera::Delta += Vector3::Normalize(Vector3(Camera::Right[0], 0.0f, Camera::Right[2])) * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);

		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_UP) == GLFW_PRESS)
			Camera::Delta += Vector3::Normalize(Vector3(Camera::Front[0], 0.0f, Camera::Front[2])) * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);

		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_DOWN) == GLFW_PRESS)
			Camera::Delta -= Vector3::Normalize(Vector3(Camera::Front[0], 0.0f, Camera::Front[2])) * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);

		Camera::Delta[1] = 0.0f;
	}

	else
	{
		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_LEFT) == GLFW_PRESS)
			Camera::Delta -= Camera::Right * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);

		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_RIGHT) == GLFW_PRESS)
			Camera::Delta += Camera::Right * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);

		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_UP) == GLFW_PRESS)
			Camera::Delta += Camera::Front * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);

		if (glfwGetKey(FrameRenderable::Interface, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(FrameRenderable::Interface, GLFW_KEY_DOWN) == GLFW_PRESS)
			Camera::Delta -= Camera::Front * MathManager::DeltaTime(MOVEMENT_SPEED * movementScalar);
	}

	if (Camera::Click[0] && Camera::Click[1])
	{
		Camera::Yaw -= MathManager::DeltaTime(LOOK_SPEED * static_cast<float>(newMouse[0] - Camera::Mouse[0]));
		Camera::Pitch += MathManager::DeltaTime(LOOK_SPEED * static_cast<float>(newMouse[1] - Camera::Mouse[1]));

		if (Camera::Pitch > 89.0f)
			Camera::Pitch = 89.0f;

		if (Camera::Pitch < -89.0f)
			Camera::Pitch = -89.0f;
	}

	Camera::Position += Camera::Delta;
	Camera::Front = Vector3::Normalize(Vector3(cosf(MathManager::Radians(Camera::Yaw)) * cosf(MathManager::Radians(Camera::Pitch)), sinf(MathManager::Radians(Camera::Pitch)), sinf(MathManager::Radians(Camera::Yaw)) * cosf(MathManager::Radians(Camera::Pitch))));
	Camera::Right = Vector3::Normalize(Vector3::Cross(Camera::Front, Vector3(0.0f, 1.0f, 0.0f)));
	Camera::Up = Vector3::Normalize(Vector3::Cross(Camera::Right, Camera::Front));
	Camera::View = Matrix4::LookAt(Camera::Position, Camera::Position + Camera::Front, Camera::Up);

	return true;
}

bool Camera::LoadWorldMatrix(const Shader& shader, const Matrix4& model)
{
	if (!shader.LoadMatrix("projection", Camera::Projection))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadMatrix("view", Camera::View))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadMatrix("model", model))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (!shader.LoadVector("viewPos", Camera::Position))
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	return true;
}