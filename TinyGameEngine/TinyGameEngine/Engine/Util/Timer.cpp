#include "Timer.h"

Timer::Timer()
{
    start = std::chrono::steady_clock::now();
    last = start;
}

float Timer::Mark()
{
    const auto old = last;
    last = std::chrono::steady_clock::now();
    const std::chrono::duration<float> frameTime = last - old;
    return frameTime.count();
}

float Timer::Peek() const
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}

float Timer::Duration() const
{
    auto d = std::chrono::steady_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count() / 1000.0f;
}
