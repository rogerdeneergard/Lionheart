#include "WinContainer.h"

WinContainer::WinContainer()
{
	static bool raw_input_initialised = false;
	if (raw_input_initialised == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			ErrorLog::Log(GetLastError(), "Failed to register raw input devices.");
			exit(-1);
		}
		raw_input_initialised = true;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinContainer::WinProc(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWin, uMsg, wParam, lParam)) return true;

	switch (uMsg)
	{
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (this->kl.GetAutoRepeatChars())
		{
			this->kl.OnChar(ch);
		}
		else if (!(lParam & (1 << 30)))
		{
			this->kl.OnChar(ch);
		}
		return 0;
	}
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (this->kl.GetAutoRepeatKeys())
		{
			this->kl.OnKeyDown(keycode);
		}
		else if (!(lParam & (1 << 30)))
		{
			this->kl.OnKeyDown(keycode);
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		this->kl.OnKeyUp(keycode);
		return 0;
	}

	// Mouse messages
	
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ml.OnLeftDown(x, y);
		return 0;
	}

	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ml.OnLeftUp(x, y);
		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ml.OnRightDown(x, y);
		return 0;
	}

	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ml.OnRightUp(x, y);
		return 0;
	}

	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ml.OnMiddleDown(x, y);
		return 0;
	}

	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ml.OnMiddleUp(x, y);
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			ml.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			ml.OnWheelDown(x, y);
		}
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ml.OnMouseMove(x, y);
		return 0;
	}
	
	case WM_INPUT:
	{
		UINT bufferSize;

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));

		if (bufferSize > 0)
		{
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(bufferSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &bufferSize, sizeof(RAWINPUTHEADER)) == bufferSize)
			{
				RAWINPUT * rawInput = reinterpret_cast<RAWINPUT *>(rawData.get());
				if (rawInput->header.dwType == RIM_TYPEMOUSE)
				{
					ml.OnMouseMoveRaw(rawInput->data.mouse.lLastX, rawInput->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(hWin, uMsg, wParam, lParam);
	}
	
	// Default message
	default:
		return DefWindowProc(hWin, uMsg, wParam, lParam);
	}
}

