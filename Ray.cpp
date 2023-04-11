#include "Camera.hpp"
#include "Input.hpp"
#include "Ray.hpp"
#include "Window.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <limits>

glm::vec3 Ray::Position, Ray::Direction;

bool Ray::Picked(const ModelMesh& model, float &distance)
{
	glm::vec4 offsetMinimum = model.Transform * glm::vec4(model.Bounds.Minimum, 1.0f), offsetMaximum = model.Transform * glm::vec4(model.Bounds.Maximum, 1.0f);
	glm::vec3 minimum = glm::min(offsetMinimum, offsetMaximum), maximum = glm::max(offsetMinimum, offsetMaximum);
	glm::vec3 delta = (minimum - Ray::Position) / Ray::Direction, gamma = (maximum - Ray::Position) / Ray::Direction;
	float minimumHit = std::fmaxf(std::fmaxf(std::fminf(delta.x, gamma.x), std::fminf(delta.y, gamma.y)), std::fminf(delta.z, gamma.z));
	float maximumHit = std::fminf(std::fminf(std::fmaxf(delta.x, gamma.x), std::fmaxf(delta.y, gamma.y)), std::fmaxf(delta.z, gamma.z));

	if (maximumHit < 0.0f)
	{
		distance = maximumHit;
		return false;
	}

	else if (minimumHit > maximumHit)
	{
		distance = std::numeric_limits<float>::max();
		return false;
	}

	distance = minimumHit;
	return true;
}

bool Ray::Update()
{
	glm::mat4 inverse = glm::inverse(Camera::Projection * glm::lookAt(glm::vec3(0.0f), Camera::Front, Camera::Up));
	glm::vec4 worldPosition;
	float mouseX = Input::CursorNowX(), mouseY = Input::CursorNowY();

	mouseX = mouseX / (Window::Width * 0.5f) - 1.0f;
	mouseY = mouseY / (Window::Height * 0.5f) - 1.0f;
	worldPosition = inverse * glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);

	Ray::Position = Camera::Position;
	Ray::Direction = glm::normalize(glm::vec3(worldPosition));

	return true;
}