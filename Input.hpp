#ifndef Input_H
#define Input_H

#include <array>
#include <chrono>

class Input
{
private:
	static std::array<uint8_t, 512> PreviousKeys, CurrentKeys;
	static std::array<uint8_t, 16> PreviousMouse, CurrentMouse;
	static std::array<double, 2> PreviousCursor, CurrentCursor;
	static std::chrono::time_point<std::chrono::steady_clock> CurrentFrame, PreviousFrame;
	static size_t DeltaFrame;
	static bool InitializeInput;

	Input() = delete;
	~Input() = delete;
	Input(const Input&) = delete;
	Input(Input&&) noexcept = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) noexcept = delete;

public:
	static const float CursorNowX();
	static const float CursorNowY();
	static const float CursorWasX();
	static const float CursorWasY();
	static const float DeltaTime(const float = 1.0f);
	static const bool KeyNowPressed(const size_t);
	static const bool KeyWasPressed(const size_t);
	static const bool MouseNowPressed(const size_t);
	static const bool MouseWasPressed(const size_t);
	static const bool Update();
};

#endif