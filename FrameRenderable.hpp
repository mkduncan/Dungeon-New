#ifndef FrameRenderable_H
#define FrameRenderable_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glfw/glfw3.h>

class FrameRenderable
{
private:
	static GLFWwindow* Interface;

	FrameRenderable() = delete;
	~FrameRenderable() = delete;
	FrameRenderable(const FrameRenderable&) = delete;
	FrameRenderable(FrameRenderable&&) noexcept = delete;
	FrameRenderable& operator=(const FrameRenderable&) = delete;
	FrameRenderable& operator=(FrameRenderable&&) noexcept = delete;

	friend class Camera;

public:
	static bool Create();
	static bool Update();
	static bool Destroy();
};

#endif