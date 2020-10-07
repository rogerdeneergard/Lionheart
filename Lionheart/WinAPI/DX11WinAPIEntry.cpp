#include "LionheartEngine.h"
#include "../Keyboard/KeyLog.h"
#include "../Util/ComException.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#include "../Graphics/Vertex.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpWStrCmdLineArgs,
	_In_ int nCmdShow)
{
	OutputDebugStringA(std::to_string(sizeof(Vertex)).c_str());
	try
	{
		HRESULT hr = CoInitialize(NULL);
	}
	catch (ComException & exception)
	{
		ErrorLog::Log(exception);
	}

	LionheartEngine main;
	try
	{
		main.Initialise(hInstance, "TestWindowClass", "Test", 1920, 1080);
	}
	catch (...)
	{
		ErrorLog::Log("Fatal engine error");
		exit(-7);
	}
	while (main.ProcessMessage() == true)
	{
		main.Update();
		main.Render();
	}
}