#pragma once
#include<string>
#include<Windows.h>
class StringConverter {
public:
	static std::wstring StrToWideStr(std::string);
};