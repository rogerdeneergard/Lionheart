#include "MouseEvent.h"

MouseEvent::MouseEvent() : type(MouseEventType::INVALID), x(0), y(0) {}

MouseEvent::MouseEvent(MouseEventType type, const int x, const int y) : type(type), x(x), y(y) {}

bool MouseEvent::IsValid() const
{
	return this->type != MouseEventType::INVALID;
}

MouseEvent::MouseEventType MouseEvent::GetType() const
{
	return this->type;
}

const Vector2<const int> MouseEvent::GetPos() const
{
	return Vector2<const int>(x, y);
}

const int MouseEvent::GetPosX() const
{
	return x;
}

const int MouseEvent::GetPosY() const
{
	return y;
}