#include "MathManager.hpp"
#include "Rectangle.hpp"

Rectangle::Rectangle() : X(0), Y(0), Width(0), Height(0)
{
}

Rectangle::~Rectangle()
{
}

Rectangle::Rectangle(const Rectangle& rectangle) : X(rectangle.X), Y(rectangle.Y), Width(rectangle.Width), Height(rectangle.Height)
{
}

Rectangle::Rectangle(Rectangle&& rectangle) : X(rectangle.X), Y(rectangle.Y), Width(rectangle.Width), Height(rectangle.Height)
{
}

Rectangle::Rectangle(const int32_t x, const int32_t y, const int32_t width, const int32_t height) : X(x), Y(y), Width(width), Height(height)
{
}

Rectangle& Rectangle::operator=(const Rectangle& rectangle)
{
	this->X = rectangle.X;
	this->Y = rectangle.Y;
	this->Width = rectangle.Width;
	this->Height = rectangle.Height;

	return (*this);
}

Rectangle& Rectangle::operator=(Rectangle&& rectangle)
{
	this->X = rectangle.X;
	this->Y = rectangle.Y;
	this->Width = rectangle.Width;
	this->Height = rectangle.Height;

	return (*this);
}