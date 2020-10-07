#pragma once
#include "../Math/Vector2.h"
#include <Windows.h>
class MouseEvent
{
public:
	enum MouseEventType
	{
		L_DOWN,
		L_UP,
		R_DOWN,
		R_UP,
		M_DOWN,
		M_UP,
		WHEELUP,
		WHEELDOWN,
		MOVE,
		RAW_MOVE,
		INVALID
	};
	MouseEvent();
	MouseEvent(const MouseEventType type, const int x, const int y);
	bool IsValid() const;
	MouseEventType GetType() const;
	const Vector2<const int> GetPos() const;
	const int GetPosX() const;
	const int GetPosY() const;
private:
	MouseEventType type;
	const int x, y;
};