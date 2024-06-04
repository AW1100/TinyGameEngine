#pragma once
#include "../CustomizedWindows.h"
#include "Window.h"
#include "../Graphics/Graphics.h"
#include "WindowsMessage.h"

#include <memory>
#include <optional>

class WindowWindows : public Window
{
	class WindowsClass;
public:
	WindowWindows(unsigned int width, unsigned int height);
	~WindowWindows();
	void Init(const char* name) override;
	void Show() override;
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();
	HWND& GetHWnd() { return hWnd; }

private:
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
	//WindowsMessage* wm_ptr = nullptr;
};

class WindowWindows::WindowsClass
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