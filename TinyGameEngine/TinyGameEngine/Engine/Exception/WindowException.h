#pragma once
#include "Exception.h"
#include "../CustomizedWindows.h"

class WindowException : public Exception
{
public:
	WindowException(int line, const char* file, HRESULT hr);
	const char* what() const override;
	virtual const char* GetType() const override;
	static std::string TranslateErrorCode(HRESULT hr);
	HRESULT GetErrorCode() const;
	std::string GetErrorString() const;

private:
	HRESULT hr;
};

#define WND_EXCEPTION(hr) WindowException(__LINE__,__FILE__,hr);