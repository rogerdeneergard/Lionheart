#pragma once
#include "WinContainer.h"
#include "../Util/Timer.h"
class LionheartEngine : WinContainer
{
public:
	bool Initialise(HINSTANCE hInstance, std::string strWinClass, std::string strWinTitle, int width, int height);
	bool ProcessMessage();
	void Update();
	void Render();

private:
	Timer timer;
};