#include "Application.h"
#include "../Math/Math.h"
#include <iostream>
#include "Event\Event.h"
#include "Window\WindowWindows.h"

TGE::Application::Application()
{
	WindowWindows wnd(800, 450);
	wnd.Init("Tiny Game Engine");
	wnd.Show();
}

TGE::Application::~Application()
{
}

int TGE::Application::Run()
{
	while (true)
	{
		if (const auto code = WindowWindows::ProcessMessages())
		{
			return *code;
		}
		DoFrame();
	}
	
}

void TGE::Application::DoFrame()
{

}

