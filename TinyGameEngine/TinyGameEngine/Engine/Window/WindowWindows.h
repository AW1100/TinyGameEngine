#pragma once
#include "Window.h"
#include <Windows.h>

class WindowsClass
{
public:
	static HINSTANCE GetInstance();
	static const char* GetName();

private:
	WindowsClass();
	~WindowsClass();
	WindowsClass(const WindowsClass&) = delete;
	WindowsClass operator=(const WindowsClass&) = delete;
private:

	static constexpr const char* wndClassName = "Tiny Game Engine";
	static WindowsClass wndClass;
	HINSTANCE hInst;
};

class WindowWindows : public Window
{
public:
	WindowWindows(unsigned int width, unsigned int height);
	~WindowWindows();
	void Init(const char* name) override;
	void Show() override;
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND hWnd;
};