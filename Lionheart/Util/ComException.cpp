#include "ComException.h"

ComException::ComException(HRESULT hr, const std::string & eMsg, const std::string & file,
	const std::string & function, const int line) 
	: 
	eMsg(L"Msg: " + StringConverter::StrToWideStr(eMsg) + L"\n" +
		_com_error(hr).ErrorMessage() + L"\n" +
		L"File: " + StringConverter::StrToWideStr(file) + L"\n" +
		L"Function: " + StringConverter::StrToWideStr(function) + L"\n" +
		L"Line: " + StringConverter::StrToWideStr(std::to_string(line))) {}

const wchar_t * ComException::GetMessage() const
{
	return eMsg.c_str();
}