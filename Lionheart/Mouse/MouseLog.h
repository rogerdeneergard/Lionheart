#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseLog
{
public:
	void OnLeftDown(const int x, const int y);
	void OnLeftUp(const int x, const int y);
	void OnRightDown(const int x, const int y);
	void OnRightUp(const int x, const int y);
	void OnMiddleDown(const int x, const int y);
	void OnMiddleUp(const int x, const int y);
	void OnWheelUp(const int x, const int y);
	void OnWheelDown(const int x, const int y);
	void OnMouseMove(const int x, const int y);
	void OnMouseMoveRaw(const int x, const int y);

	bool IsLeftDown() const;
	bool IsRightDown() const;
	bool IsMiddleDown() const;

	Vector2<int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

	bool EventQueueIsEmpty() const;
	MouseEvent NextEvent(const bool remove = true);

private:
	std::queue<MouseEvent> eventQueue;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool middleIsDown = false;
	int x, y;
};