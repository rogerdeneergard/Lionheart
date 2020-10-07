#include"StringConverter.h"

std::wstring StringConverter::StrToWideStr(std::string source) {
	std::wstring result(source.begin(), source.end());
	return result;
}