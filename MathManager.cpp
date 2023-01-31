#include "MathManager.hpp"

std::mt19937_64 MathManager::RandomGenerator;
std::chrono::time_point<std::chrono::steady_clock> MathManager::CurrentFrame, MathManager::PreviousFrame;
size_t MathManager::RandomSeed = MathManager::Reseed(std::chrono::high_resolution_clock::now().time_since_epoch().count()), MathManager::DeltaFrame = 0;
bool MathManager::InitializeDeltaTime = true;

size_t MathManager::Reseed(const size_t seedValue)
{
	MathManager::RandomGenerator.seed(seedValue);
	MathManager::RandomSeed = seedValue;

	return seedValue;
}

bool MathManager::UpdateDeltaTime()
{
	if (MathManager::InitializeDeltaTime)
	{
		MathManager::CurrentFrame = std::chrono::high_resolution_clock::now();
		MathManager::InitializeDeltaTime = false;
	}

	else
	{
		MathManager::PreviousFrame = MathManager::CurrentFrame;
		MathManager::CurrentFrame = std::chrono::high_resolution_clock::now();
		MathManager::DeltaFrame = std::chrono::duration_cast<std::chrono::microseconds>(MathManager::CurrentFrame - MathManager::PreviousFrame).count();
	}

	return true;
}

float MathManager::Clamp(const float value, const float minimum, const float maximum)
{
	if (minimum > maximum)
		return MathManager::Clamp(value, maximum, minimum);

	return std::fminf(std::fmaxf(value, minimum), maximum);
}

float MathManager::DeltaTime(const float scaleValue)
{
	static const size_t maximumDelay = 1000000;
	static const double inverseDelay = 1.0 / static_cast<double>(maximumDelay);

	if (MathManager::InitializeDeltaTime || MathManager::DeltaFrame > maximumDelay)
		return 0.0f;

	return static_cast<float>(static_cast<double>(scaleValue) * static_cast<double>(MathManager::DeltaFrame) * inverseDelay);
}

float MathManager::Radians(const float scalarValue)
{
	return scalarValue * 1.74532925e-2f;
}

int32_t MathManager::RandomSigned(const int32_t minimumBound, const int32_t maximumBound)
{
	if (minimumBound > maximumBound)
		return MathManager::RandomSigned(maximumBound, minimumBound);

	std::uniform_int_distribution<int32_t> distribution(minimumBound, maximumBound);
	return distribution(MathManager::RandomGenerator);
}

size_t MathManager::RandomUnsigned(const size_t minimumBound, const size_t maximumBound)
{
	if (minimumBound > maximumBound)
		return MathManager::RandomUnsigned(maximumBound, minimumBound);

	std::uniform_int_distribution<size_t> distribution(minimumBound, maximumBound);
	return distribution(MathManager::RandomGenerator);
}

float MathManager::RandomReal(const float minimumBound, const float maximumBound)
{
	if (minimumBound > maximumBound)
		return MathManager::RandomReal(maximumBound, minimumBound);

	std::uniform_real_distribution<float> distribution(minimumBound, maximumBound);
	return distribution(MathManager::RandomGenerator);
}

bool MathManager::RandomBoolean(const float probability)
{
	std::bernoulli_distribution distribution(probability);
	return distribution(MathManager::RandomGenerator);
}

GLuint MathManager::MoveScalar(GLuint& scalar)
{
	GLuint result = scalar;

	scalar = 0;
	return result;
}

size_t MathManager::MoveScalar(size_t& scalar)
{
	size_t result = scalar;

	scalar = 0;
	return result;
}

GLfloat MathManager::MoveReal(GLfloat& scalar)
{
	GLfloat result = scalar;

	scalar = 0.0f;
	return result;
}