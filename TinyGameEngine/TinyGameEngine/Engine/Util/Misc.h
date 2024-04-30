#pragma once
#include <cwchar>
#include <codecvt>
#include <string>
#include "stringapiset.h"

wchar_t* StringToWideString(const char* str)
{
	int bufferLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
	wchar_t* wideString = new wchar_t[bufferLen];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wideString, bufferLen);
	return wideString;
}

