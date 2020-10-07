#pragma once
#include<string>
#include<Windows.h>
#include"ComException.h"
class ErrorLog {
public:
	static void Log(std::string msg);
	static void Log(HRESULT hr, std::string msg);
	static void Log(HRESULT hr, std::wstring msg);
	static void Log(ComException exception);
};