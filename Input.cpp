#include "Input.hpp"
#include "Logger.hpp"
#include "Window.hpp"

std::array<uint8_t, 512> Input::PreviousKeys, Input::CurrentKeys;
std::array<uint8_t, 16> Input::PreviousMouse, Input::CurrentMouse;
std::array<double, 2> Input::PreviousCursor, Input::CurrentCursor;
std::chrono::time_point<std::chrono::steady_clock> Input::CurrentFrame, Input::PreviousFrame;
size_t Input::DeltaFrame = 0;
bool Input::InitializeInput = true;

const float Input::CursorNowX()
{
	if (Window::Interface == nullptr)
	{
		Logger::LogError(__FUNCTION__, __LINE__);
		return -0.0f;
	}

	return static_cast<float>(Input::CurrentCursor[0]);
}

const float Input::CursorNowY()
{
	if (Window::Interface == nullptr)
	{
		Logger::LogError(__FUNCTION__, __LINE__);
		return -0.0f;
	}

	return static_cast<float>(Input::CurrentCursor[1]);
}

const float Input::CursorWasX()
{
	if (Window::Interface == nullptr)
	{
		Logger::LogError(__FUNCTION__, __LINE__);
		return -0.0f;
	}

	return static_cast<float>(Input::PreviousCursor[0]);
}

const float Input::CursorWasY()
{
	if (Window::Interface == nullptr)
	{
		Logger::LogError(__FUNCTION__, __LINE__);
		return -0.0f;
	}

	return static_cast<float>(Input::PreviousCursor[1]);
}

const float Input::Input::DeltaTime(const float scaleValue)
{
	const size_t maximumDelay = 1000000;

	if (Window::Interface == nullptr || Input::InitializeInput)
	{
		Logger::LogError(__FUNCTION__, __LINE__);
		return -0.0f;
	}

	else if (Input::DeltaFrame > maximumDelay)
		return 0.0f;

	return static_cast<float>(static_cast<double>(scaleValue) * static_cast<double>(Input::DeltaFrame) / static_cast<double>(maximumDelay));
}

const bool Input::KeyNowPressed(const size_t keyIndex)
{
	if (Window::Interface == nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (glfwGetKey(Window::Interface, static_cast<int>(keyIndex)) == GLFW_PRESS)
	{
		Input::CurrentKeys[keyIndex] = GLFW_PRESS;
		return true;
	}

	Input::CurrentKeys[keyIndex] = GLFW_RELEASE;
	return false;
}

const bool Input::KeyWasPressed(const size_t keyIndex)
{
	if (Window::Interface == nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	return Input::PreviousKeys[keyIndex] == GLFW_PRESS;
}

const bool Input::MouseNowPressed(const size_t mouseIndex)
{
	if (Window::Interface == nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	if (glfwGetMouseButton(Window::Interface, static_cast<int>(mouseIndex)) == GLFW_PRESS)
	{
		Input::CurrentMouse[mouseIndex] = GLFW_PRESS;
		return true;
	}

	Input::CurrentMouse[mouseIndex] = GLFW_RELEASE;
	return false;
}

const bool Input::MouseWasPressed(const size_t mouseIndex)
{
	if (Window::Interface == nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	return Input::PreviousMouse[mouseIndex] == GLFW_PRESS;
}

const bool Input::Update()
{
	if (Window::Interface == nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (InitializeInput)
	{
		InitializeInput = false;
		Input::CurrentFrame = std::chrono::high_resolution_clock::now();

		for (size_t index = 0; index < Input::PreviousKeys.size(); ++index)
		{
			Input::PreviousKeys[index] = GLFW_RELEASE;
			Input::CurrentKeys[index] = GLFW_RELEASE;
		}

		for (size_t index = 0; index < Input::PreviousMouse.size(); ++index)
		{
			Input::PreviousMouse[index] = GLFW_RELEASE;
			Input::CurrentMouse[index] = GLFW_RELEASE;
		}

		for (size_t index = 0; index < Input::PreviousCursor.size(); ++index)
		{
			Input::PreviousCursor[index] = -0.0;
			Input::CurrentCursor[index] = -0.0;
		}
	}

	else
	{
		Input::PreviousKeys = Input::CurrentKeys;
		Input::PreviousMouse = Input::CurrentMouse;
		Input::PreviousCursor = Input::CurrentCursor;
		Input::PreviousFrame = Input::CurrentFrame;
		Input::CurrentFrame = std::chrono::high_resolution_clock::now();
		Input::DeltaFrame = std::chrono::duration_cast<std::chrono::microseconds>(Input::CurrentFrame - Input::PreviousFrame).count();

		glfwGetCursorPos(Window::Interface, &Input::CurrentCursor[0], &Input::CurrentCursor[1]);
	}

	return true;
}