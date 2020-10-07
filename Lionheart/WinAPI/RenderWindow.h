#pragma once
#include "../Util/ErrorLog.h"

class WinContainer;

class RenderWindow {
public:
	bool Initialise(WinContainer * pWinContainer, HINSTANCE, std::string, std::string, int, int);
	bool ProcessMessage();
	HWND GetHWin() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND handle = NULL;
	HINSTANCE hInstance = NULL;
	std::string strWinClass = "";
	std::wstring wStrWinClass = L"";
	std::string strWinTitle = "";
	std::wstring wStrWinTitle = L"";
	int width = 0;
	int height = 0;
};

LRESULT CALLBACK WinProcInitialise(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcMsgRedirect(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);