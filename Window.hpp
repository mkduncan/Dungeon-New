#ifndef Window_H
#define Window_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glfw/glfw3.h>

class Window
{
public:
	static GLFWwindow* Interface;
	static float Width, Height;

	Window() = delete;
	~Window() = delete;
	Window(const Window&) = delete;
	Window(Window&&) noexcept = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) noexcept = delete;

	static const bool Create();
	static const bool Destroy();
	static const bool Update();
};

#endif