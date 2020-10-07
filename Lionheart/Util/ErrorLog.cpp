#include"ErrorLog.h"
#include<comdef.h>

void ErrorLog::Log(std::string msg) {
	std::string display = "Error - " + msg;
	MessageBoxA(NULL, display.c_str(), "Error", MB_ICONERROR);
}

void ErrorLog::Log(HRESULT hr, std::string msg) 
{
	_com_error error(hr);
	std::wstring display = L"Error - " + StringConverter::StrToWideStr(msg) + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, display.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLog::Log(HRESULT hr, std::wstring msg)
{
	_com_error error(hr);
	std::wstring display = L"Error - " + msg + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, display.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLog::Log(ComException exception)
{
	MessageBoxW(NULL, exception.GetMessageW(), L"Error", MB_ICONERROR);
}
