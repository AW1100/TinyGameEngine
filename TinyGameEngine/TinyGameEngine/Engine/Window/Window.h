#pragma once

class Window
{
public:
	Window(unsigned int w, unsigned int h) :
		width(w), height(h)
	{}
	~Window() = default;
	virtual void Init(const char* name) = 0;
	virtual void Show() = 0;
protected:
	unsigned int width;
	unsigned int height;
};
