#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	float Mark();
	float Peek() const;
	float Duration() const;

private:
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point last;
};