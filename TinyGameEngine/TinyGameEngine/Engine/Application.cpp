#include "Application.h"
#include "../Math/Math.h"
#include "Event/Event.h"

#include <iostream>

TGE::Application::Application()
{
	wnd = new WindowWindows(800, 450);
	wnd->Init("Tiny Game Engine");
	wnd->Show();
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
	wnd->Gfx().ClearBuffer(0.0f, 0.0f, 1.0f);
	wnd->Gfx().EndFrame();
}

