#include "Input.hpp"
#include "Logger.hpp"
#include "Window.hpp"

#define WINDOW_TITLE "Dungeon"

GLFWwindow* Window::Interface = nullptr;
float Window::Width = 0.0f, Window::Height = 0.0f;

const bool Window::Create()
{
	GLFWmonitor* monitor = nullptr;
	int32_t monitorX = 0, monitorY = 0, width = 0, height = 0;

	if (Window::Interface != nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (glfwInit() == GLFW_FALSE)
		return Logger::LogError(__FUNCTION__, __LINE__);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

	if (videoMode == nullptr)
	{
		glfwTerminate();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	width = (videoMode->width * 3) >> 2;
	height = (videoMode->height * 3) >> 2;

	if (width < height)
		height = width;

	else
		width = height;

	Window::Interface = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);

	if (Window::Interface == nullptr)
	{
		glfwTerminate();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	glfwGetMonitorPos(monitor, &monitorX, &monitorY);
	glfwSetWindowPos(Window::Interface, monitorX + ((videoMode->width - width) >> 1), monitorY + ((videoMode->height - height) >> 1));
	glfwMakeContextCurrent(Window::Interface);
	glfwSwapInterval(1);

	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == GLFW_FALSE)
	{
		glfwDestroyWindow(Window::Interface);
		glfwTerminate();
		Window::Interface = nullptr;

		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	Window::Width = static_cast<float>(width);
	Window::Height = static_cast<float>(height);

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Window::Destroy()
{
	if (Window::Interface != nullptr)
		glfwDestroyWindow(Window::Interface);

	glfwTerminate();
	Window::Interface = nullptr;

	return true;
}

const bool Window::Update()
{
	if (Window::Interface == nullptr)
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (glfwWindowShouldClose(Window::Interface) == GLFW_TRUE)
		return false;

	glfwSwapBuffers(Window::Interface);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (!Logger::LogErrorGL(__FUNCTION__, __LINE__))
		return false;

	return Input::Update();
}