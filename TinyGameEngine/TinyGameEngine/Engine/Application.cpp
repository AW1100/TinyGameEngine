#include "Application.h"
#include "../Math/Math.h"
#include <iostream>
#include "Event\Event.h"
#include "Window\WindowWindows.h"

TGE::Application::Application()
{
}

TGE::Application::~Application()
{
}

void TGE::Application::Run()
{
	WindowWindows wnd(800, 450);
	wnd.Init("Tiny Game Engine");
	wnd.Show();

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
}

