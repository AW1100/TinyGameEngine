#pragma once

#ifdef TGE_PLATFORM_WINDOWS
#include <Windows.h>

TGE::Application* TGE::CreateApplication();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int mCmdShow)
{
	auto app = TGE::CreateApplication();
	app->Run();
	delete app;
}

#endif