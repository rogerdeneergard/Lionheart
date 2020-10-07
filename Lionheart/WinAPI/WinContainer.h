#pragma once
#include "RenderWindow.h"
#include "../Keyboard/KeyLog.h"
#include "../Mouse/MouseLog.h"
#include "../Graphics/GraphicsHandler.h"
class WinContainer
{
public:
	WinContainer();
	LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
protected:
	RenderWindow rw;
	KeyLog kl;
	MouseLog ml;
	GraphicsHandler gfx;
};