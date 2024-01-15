#include "Application.h"
#include "../Math/Math.h"
#include "Event/Event.h"

#include <iostream>

TGE::Application::Application()
{
	wnd = new WindowWindows(800, 450);
	wnd->Init("Tiny Game Engine");
	wnd->Show();
	scene = new Scene(wnd->Gfx());
	wnd->Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

TGE::Application::~Application()
{
	delete wnd;
	delete scene;
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
	auto dt = timer.Mark();
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << timer.Duration();
	SetWindowText(wnd->GetHWnd(), ("Tiny Game Engine " + oss.str() + " s").c_str());
	wnd->Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	scene->UpdateFrame(dt, wnd->Gfx());
	wnd->Gfx().EndFrame();
}

