#include"MouseLog.h"

void MouseLog::OnLeftDown(const int x, const int y)
{
	leftIsDown = true;
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::L_DOWN, x, y));
}

void MouseLog::OnLeftUp(const int x, const int y)
{
	leftIsDown = false;
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::L_UP, x, y));
}

void MouseLog::OnRightDown(const int x, const int y)
{
	rightIsDown = true;
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::R_DOWN, x, y));
}

void MouseLog::OnRightUp(const int x, const int y)
{
	rightIsDown = false;
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::R_UP, x, y));
}

void MouseLog::OnMiddleDown(const int x, const int y)
{
	middleIsDown = true;
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::M_DOWN, x, y));
}

void MouseLog::OnMiddleUp(const int x, const int y)
{
	middleIsDown = false;
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::M_UP, x, y));
}

void MouseLog::OnWheelUp(const int x, const int y)
{
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::WHEELUP, x, y));
}

void MouseLog::OnWheelDown(const int x, const int y)
{
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::WHEELDOWN, x, y));
}

void MouseLog::OnMouseMove(const int x, const int y)
{
	this->x = x; 
	this->y = y;
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::MOVE, x, y));
}

void MouseLog::OnMouseMoveRaw(const int x, const int y)
{
	eventQueue.push(MouseEvent(MouseEvent::MouseEventType::RAW_MOVE, x, y));
}

bool MouseLog::IsLeftDown() const
{
	return leftIsDown;
}

bool MouseLog::IsRightDown() const
{
	return rightIsDown;
}

bool MouseLog::IsMiddleDown() const
{
	return middleIsDown;
}

Vector2<int> MouseLog::GetPos() const
{
	return Vector2<int>(x, y);
}

int MouseLog::GetPosX() const
{
	return x;
}

int MouseLog::GetPosY() const
{
	return y;
}

bool MouseLog::EventQueueIsEmpty() const{
	return eventQueue.empty();
}

MouseEvent MouseLog::NextEvent(const bool remove)
{
	if (eventQueue.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = eventQueue.front();
		if (remove) eventQueue.pop();
		return e;
	}
}