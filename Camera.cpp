#include "Camera.hpp"
#include "DungeonModel.hpp"
#include "Frustum.hpp"
#include "Input.hpp"
#include "Logger.hpp"
#include "Ray.hpp"

#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#define SPEED_MULTIPLIER 4.0f

glm::mat4 Camera::Projection(glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 32.0f)), Camera::View(glm::lookAt(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
glm::vec3 Camera::Position(0.0f, DungeonModel::MeshHeight * 0.75f, 0.0f), Camera::Delta, Camera::Right(glm::vec3(1.0f, 0.0f, 0.0f)), Camera::Up(glm::vec3(0.0f, 1.0f, 0.0f)), Camera::Front(glm::vec3(0.0f, 0.0f, -1.0f));
float Camera::Pitch = 0.0f, Camera::Yaw = -90.0f, Camera::MovementSpeed = 6.0f, Camera::LookSpeed = 36.0f;

const bool Camera::Enable(const Shader& shader, const glm::mat4& modelMatrix)
{
	if (!shader.Enable())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadMatrix("projection", Camera::Projection))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadMatrix("view", Camera::View))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadMatrix("model", modelMatrix))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!shader.UploadVector("viewPos", Camera::Position))
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}

const bool Camera::Update(const bool collide, const bool lockY, const bool lockSpeed)
{
	float movementScalar = 1.0f;
	Camera::Delta = glm::vec3();

	if (!lockSpeed && (Input::KeyNowPressed(GLFW_KEY_LEFT_SHIFT) || Input::KeyNowPressed(GLFW_KEY_RIGHT_SHIFT)))
		movementScalar = SPEED_MULTIPLIER;

	if (lockY)
	{
		if (Input::KeyNowPressed(GLFW_KEY_A) || Input::KeyNowPressed(GLFW_KEY_LEFT))
			Camera::Delta -= glm::normalize(glm::vec3(Camera::Right[0], 0.0f, Camera::Right[2])) * Input::DeltaTime(Camera::MovementSpeed * movementScalar);

		if (Input::KeyNowPressed(GLFW_KEY_D) || Input::KeyNowPressed(GLFW_KEY_RIGHT))
			Camera::Delta += glm::normalize(glm::vec3(Camera::Right[0], 0.0f, Camera::Right[2])) * Input::DeltaTime(Camera::MovementSpeed * movementScalar);

		if (Input::KeyNowPressed(GLFW_KEY_W) || Input::KeyNowPressed(GLFW_KEY_UP))
			Camera::Delta += glm::normalize(glm::vec3(Camera::Front[0], 0.0f, Camera::Front[2])) * Input::DeltaTime(Camera::MovementSpeed * movementScalar);

		if (Input::KeyNowPressed(GLFW_KEY_S) || Input::KeyNowPressed(GLFW_KEY_DOWN))
			Camera::Delta -= glm::normalize(glm::vec3(Camera::Front[0], 0.0f, Camera::Front[2])) * Input::DeltaTime(Camera::MovementSpeed * movementScalar);

		Camera::Position.y = DungeonModel::MeshHeight * 0.625f;
	}

	else
	{
		if (Input::KeyNowPressed(GLFW_KEY_A) || Input::KeyNowPressed(GLFW_KEY_LEFT))
			Camera::Delta -= Camera::Right * Input::DeltaTime(Camera::MovementSpeed * movementScalar);

		if (Input::KeyNowPressed(GLFW_KEY_D) || Input::KeyNowPressed(GLFW_KEY_RIGHT))
			Camera::Delta += Camera::Right * Input::DeltaTime(Camera::MovementSpeed * movementScalar);

		if (Input::KeyNowPressed(GLFW_KEY_W) || Input::KeyNowPressed(GLFW_KEY_UP))
			Camera::Delta += Camera::Front * Input::DeltaTime(Camera::MovementSpeed * movementScalar);

		if (Input::KeyNowPressed(GLFW_KEY_S) || Input::KeyNowPressed(GLFW_KEY_DOWN))
			Camera::Delta -= Camera::Front * Input::DeltaTime(Camera::MovementSpeed * movementScalar);
	}

	if (Input::MouseNowPressed(GLFW_MOUSE_BUTTON_1) && Input::MouseWasPressed(GLFW_MOUSE_BUTTON_1))
	{
		Camera::Yaw += Input::DeltaTime(Camera::LookSpeed * (Input::CursorNowX() - Input::CursorWasX()));
		Camera::Pitch -= Input::DeltaTime(Camera::LookSpeed * (Input::CursorNowY() - Input::CursorWasY()));
		Camera::Yaw = glm::mod(Camera::Yaw, 360.0f);
		Camera::Pitch = glm::clamp(Camera::Pitch, -89.5f, 89.5f);
	}

	if (collide && !DungeonModel::CollideCamera())
		return Logger::LogError(__FUNCTION__, __LINE__);

	Camera::Position += Camera::Delta;
	Camera::Front = glm::normalize(glm::vec3(glm::cos(glm::radians(Camera::Yaw)) * glm::cos(glm::radians(Camera::Pitch)), glm::sin(glm::radians(Camera::Pitch)), glm::sin(glm::radians(Camera::Yaw)) * glm::cos(glm::radians(Camera::Pitch))));
	Camera::Right = glm::normalize(glm::cross(Camera::Front, glm::vec3(0.0f, 1.0f, 0.0f)));
	Camera::Up = glm::normalize(glm::cross(Camera::Right, Camera::Front));
	Camera::View = glm::lookAt(Camera::Position, Camera::Position + Camera::Front, Camera::Up);

	if (!Frustum::Update())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!Ray::Update())
		return Logger::LogError(__FUNCTION__, __LINE__);

	return true;
}