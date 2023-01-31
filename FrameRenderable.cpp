#include "FrameRenderable.hpp"
#include "MathManager.hpp"
#include "TraceManager.hpp"

#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE "Game"

GLFWwindow* FrameRenderable::Interface = nullptr;

bool FrameRenderable::Create()
{
	GLFWmonitor* monitor = nullptr;
	int32_t monitorX = 0, monitorY = 0;

	if (FrameRenderable::Interface != nullptr)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (glfwInit() == GLFW_FALSE)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	FrameRenderable::Interface = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);

	if (FrameRenderable::Interface == nullptr)
	{
		glfwTerminate();
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);
	}

	monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

	if (videoMode == nullptr)
	{
		glfwTerminate();
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);
	}

	glfwGetMonitorPos(monitor, &monitorX, &monitorY);
	glfwSetWindowPos(FrameRenderable::Interface, monitorX + ((videoMode->width - WINDOW_WIDTH) >> 1), monitorY + ((videoMode->height - WINDOW_HEIGHT) >> 1));
	glfwMakeContextCurrent(FrameRenderable::Interface);
	glfwSwapInterval(1);

	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == GLFW_FALSE)
	{
		glfwDestroyWindow(FrameRenderable::Interface);
		glfwTerminate();
		FrameRenderable::Interface = nullptr;

		return TraceManager::AddMessage(__FUNCTION__, __LINE__);
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return TraceManager::CheckErrorGL(__FUNCTION__, __LINE__);
}

bool FrameRenderable::Update()
{
	if (FrameRenderable::Interface == nullptr)
		return TraceManager::AddMessage(__FUNCTION__, __LINE__);

	else if (glfwWindowShouldClose(FrameRenderable::Interface) == GLFW_TRUE)
		return false;

	glfwSwapBuffers(FrameRenderable::Interface);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!TraceManager::CheckErrorGL(__FUNCTION__, __LINE__))
		return false;

	return MathManager::UpdateDeltaTime();
}

bool FrameRenderable::Destroy()
{
	if (FrameRenderable::Interface != nullptr)
		glfwDestroyWindow(FrameRenderable::Interface);

	glfwTerminate();
	FrameRenderable::Interface = nullptr;

	return true;
}