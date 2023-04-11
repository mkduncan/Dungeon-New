#ifndef Rectangle_H
#define Rectangle_H

#include <cstdint>
#include <glm/glm.hpp>

class Rectangle
{
public:
	int32_t X, Y, Width, Height;

	Rectangle();
	~Rectangle();
	Rectangle(const Rectangle&);
	Rectangle(Rectangle&&) noexcept;
	Rectangle(const int32_t, const int32_t, const int32_t, const int32_t);
	Rectangle& operator=(const Rectangle&);
	Rectangle& operator=(Rectangle&&) noexcept;

	const bool Contains(const glm::vec2&) const;
};

#endif