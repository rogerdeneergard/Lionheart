#pragma once

#include <comdef.h>
#include "StringConverter.h"

#define COM_CHECK_FAIL(hr, msg) if (FAILED(hr)) throw ComException(hr, msg, __FILE__, __FUNCTION__, __LINE__)

class ComException
{
public:
	ComException(HRESULT hr, const std::string & eMsg, const std::string & file,
		const std::string & function, int line);
	const wchar_t * GetMessage() const;
private:
	std::wstring eMsg;
};