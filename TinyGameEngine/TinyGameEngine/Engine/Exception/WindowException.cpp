#include "WindowException.h"

#include <sstream>

WindowException::WindowException(int line, const char* file, HRESULT hr)
	:Exception(line,file),hr(hr)
{}

const char* WindowException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* WindowException::GetType() const
{
	return "WindowException";
}

std::string WindowException::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf),
		0,
		nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unknown Error Code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT WindowException::GetErrorCode() const
{
	return hr;
}

std::string WindowException::GetErrorString() const
{
	return TranslateErrorCode(hr);
}


