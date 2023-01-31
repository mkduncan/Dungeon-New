#ifndef MathManager_H
#define MathManager_H

#include <chrono>
#include <glad/glad.h>
#include <limits>
#include <random>

class FrameRenderable;

class MathManager
{
private:
	static std::mt19937_64 RandomGenerator;
	static std::chrono::time_point<std::chrono::steady_clock> CurrentFrame, PreviousFrame;
	static size_t RandomSeed, DeltaFrame;
	static bool InitializeDeltaTime;

	MathManager() = delete;
	~MathManager() = delete;
	MathManager(const MathManager&) = delete;
	MathManager(MathManager&&) noexcept = delete;
	MathManager& operator=(const MathManager&) = delete;
	MathManager& operator=(MathManager&&) noexcept = delete;

	static size_t Reseed(const size_t);
	static bool UpdateDeltaTime();

	friend class FrameRenderable;

public:
	static float Clamp(const float, const float, const float);
	static float DeltaTime(const float = 1.0f);
	static float Radians(const float);
	static int32_t RandomSigned(const int32_t = std::numeric_limits<int32_t>::lowest(), const int32_t = std::numeric_limits<int32_t>::max());
	static size_t RandomUnsigned(const size_t = std::numeric_limits<size_t>::lowest(), const size_t = std::numeric_limits<size_t>::max());
	static float RandomReal(const float = std::numeric_limits<float>::lowest(), const float = std::numeric_limits<float>::max());
	static bool RandomBoolean(const float = 0.5f);
	static GLuint MoveScalar(GLuint&);
	static size_t MoveScalar(size_t&);
	static GLfloat MoveReal(GLfloat&);
};

#endif