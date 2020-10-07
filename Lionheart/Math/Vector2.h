#pragma once

template <typename T>
class Vector2
{
public:
	Vector2() : x((T)0), y((T)0) {}
	Vector2(T x, T y) : x(x), y(y) {}
	int GetX() const { return this->x; }
	int GetY() const { return this->y; }
	~Vector2();
private:
	T x;
	T y;
};