#include "WindowWindows.h"
#include "../Exception/WindowException.h"
#include "../Event/EventHandler.h"
#include "../Event/KeyboardEvent.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <iostream>

WindowWindows::WindowsClass WindowWindows::WindowsClass::wndClass;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WindowWindows::WindowsClass::WindowsClass()
{
	hInst = GetModuleHandle(nullptr);
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowWindows::HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	RegisterClassEx(&wc);
}

WindowWindows::WindowsClass::~WindowsClass()
{
	UnregisterClass(wndClassName, hInst);
}

HINSTANCE WindowWindows::WindowsClass::GetInstance()
{
	return wndClass.hInst;
}

const char* WindowWindows::WindowsClass::GetName()
{
	return wndClassName;
}


WindowWindows::WindowWindows(unsigned int width, unsigned int height) :
	Window(width,height)
{
	hWnd = nullptr;
}

WindowWindows::~WindowWindows()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	
	DestroyWindow(hWnd);
}

void WindowWindows::Init(const char* name)
{
	RECT wr;
	wr.left = 100;
	wr.right = wr.left + width;
	wr.top = 100;
	wr.bottom = wr.top + height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	hWnd = CreateWindow(WindowsClass::GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr,
		nullptr,
		WindowsClass::GetInstance(),
		this);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	pGfx = std::make_unique<Graphics>(hWnd);

	ImGui_ImplWin32_Init(hWnd);
}

void WindowWindows::Show()
{
	
}

LRESULT WindowWindows::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowWindows* const pWnd = static_cast<WindowWindows*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowWindows::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT __stdcall WindowWindows::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowWindows* const pWnd = reinterpret_cast<WindowWindows*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT WindowWindows::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::ostringstream oss;
#if TGE_WM_DEBUG
	oss << "Message: " << WindowsMessage::GetInstance().Find(msg) << "\n";
	OutputDebugStringA(oss.str().c_str());
#endif

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

#if TGE_WM_DEBUG
	case WM_LBUTTONDOWN:
		const POINTS pt = MAKEPOINTS(lParam);
		oss << "(" << pt.x << "," << pt.y << ")\n";
		OutputDebugStringA(oss.str().c_str());
#endif

	case WM_KEYDOWN:
		new TGE::KeyboardEvent(TGE::EventType::KeyPressed, static_cast<unsigned char>(wParam));
		break;

	case WM_KEYUP:
		new TGE::KeyboardEvent(TGE::EventType::KeyReleased, static_cast<unsigned char>(wParam));
		break;
 	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> WindowWindows::ProcessMessages()
{
	try 
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return {};
	}
	catch(const WindowException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "Unknown", "Unknown", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
	
}

Graphics& WindowWindows::Gfx()
{
	return *pGfx;
}
