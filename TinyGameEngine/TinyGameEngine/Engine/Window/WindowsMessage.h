#pragma once
#include "../CustomizedWindows.h"

#include <unordered_map>
#include <string>
#include <sstream>

class WindowsMessage
{
public:
	static WindowsMessage& GetInstance();
	int NumOfRegisteredMsg() const;
	std::string Find(UINT msg) const;

private:
	WindowsMessage();
	WindowsMessage(const WindowsMessage&) = delete;
	WindowsMessage& operator=(const WindowsMessage&) = delete;
	~WindowsMessage() = default;
	
	std::unordered_map<UINT, std::string> messageMap;
};

