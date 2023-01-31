#ifndef Rectangle_H
#define Rectangle_H

#include <cstdint>

class Rectangle
{
private:
	int32_t X, Y, Width, Height;
	friend class DungeonData;

public:
	Rectangle();
	~Rectangle();
	Rectangle(const Rectangle&);
	Rectangle(Rectangle&&);
	Rectangle(const int32_t, const int32_t, const int32_t, const int32_t);
	Rectangle& operator=(const Rectangle&);
	Rectangle& operator=(Rectangle&&);
};

#endif