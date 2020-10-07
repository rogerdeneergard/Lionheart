#include"RenderWindow.h"
#include"WinContainer.h"

bool RenderWindow::Initialise(WinContainer * pWinContainer, HINSTANCE hInstance, std::string strWinClass, std::string strWinTitle, int width, int height)
{
	this->hInstance = hInstance;
	this->strWinClass = strWinClass;
	this->wStrWinClass = StringConverter::StrToWideStr(strWinClass);
	this->strWinTitle = strWinTitle;
	this->wStrWinTitle = StringConverter::StrToWideStr(strWinTitle);
	this->width = width;
	this->height = height;

	this->RegisterWindowClass();

	RECT winRect;
	winRect.left = (GetSystemMetrics(SM_CXSCREEN) - this->width) / 2;
	winRect.top = (GetSystemMetrics(SM_CYSCREEN) - this->height) / 2;
	winRect.right = winRect.left + this->width;
	winRect.bottom = winRect.top + this->height;
	AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	this->handle = CreateWindowEx(
		0,
		this->wStrWinClass.c_str(),
		this->wStrWinTitle.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		winRect.left,
		winRect.top,
		winRect.right - winRect.left,
		winRect.bottom - winRect.top,
		NULL,
		NULL,
		this->hInstance,
		pWinContainer
	);

	if (handle == NULL)
	{
		ErrorLog::Log(GetLastError(), "CreateWindowEx failed for window \'" + this->strWinTitle + "\'");
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

LRESULT CALLBACK WinProcInitialise(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW * const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
		WinContainer * pWinContainer = reinterpret_cast<WinContainer *>(pCreate->lpCreateParams);
		if (pWinContainer == nullptr)
		{
			ErrorLog::Log("Window container pointer is null at window creation");
			exit(-1);
		}
		SetWindowLongPtr(hWin, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWinContainer));
		SetWindowLongPtr(hWin, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ProcMsgRedirect));
		return pWinContainer->WinProc(hWin, uMsg, wParam, lParam);
	}
	case WM_CHAR: 
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		return 0;
	}
	default:
		return DefWindowProc(hWin, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK ProcMsgRedirect(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWin);
		return 0;
	default:
	{
		WinContainer * const pWinContainer = reinterpret_cast<WinContainer *>(GetWindowLongPtr(hWin, GWLP_USERDATA));
		return pWinContainer->WinProc(hWin, uMsg, wParam, lParam);
	}
	}
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WinProcInitialise;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = this->wStrWinClass.c_str();
	wc.lpszMenuName = NULL;

	RegisterClassEx(&wc);
}

bool RenderWindow::ProcessMessage()
{
	// Initialise and clear memory of message container
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Check for message on the queue, if there is one, translate, dispatch, and remove it from the queue
	while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Check if the latest message is null, indicating failure to respond or closure of the window
	if (msg.message == WM_NULL)
	{
		// Check if the window is closed
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; // Message loop will destroy the window, just null the handle
			UnregisterClass(this->wStrWinClass.c_str(), this->hInstance); // Unregister the window class
			return false;
		}
	}

	// Continue looping if there are no anomalies
	return true;
}

HWND RenderWindow::GetHWin() const
{
	return this->handle;
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->wStrWinClass.c_str(), this->hInstance);
		DestroyWindow(this->handle);
	}
}