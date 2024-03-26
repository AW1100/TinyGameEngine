#include "Application.h"
#include "../Math/Math.h"
#include "Event/Event.h"
#include "Constants.h"

#include <iostream>
#include <d3d11sdklayers.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

TGE::Application::Application()
{
	wnd = new WindowWindows(WIDTH, HEIGHT);
	wnd->Init("Tiny Game Engine");
	wnd->Show();
	scene = new Scene(wnd->Gfx());
	wnd->Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));

}

TGE::Application::~Application()
{
	if (wnd != nullptr)
	{
		delete wnd;
	}
	if (scene != nullptr)
	{
		delete scene;
	}
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
	auto dt = timer.Mark() * speed_factor;
	wnd->Gfx().SetCamera(cam.GetView());
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << timer.Duration();
	SetWindowText(wnd->GetHWnd(), ("Tiny Game Engine " + oss.str() + " s").c_str());
	wnd->Gfx().ClearBuffer(0.1f, 0.1f, 0.1f);
	scene->UpdateFrame(dt, wnd->Gfx());
	
	wnd->Gfx().BeginImgui(speed_factor);
	cam.SpawnControlWindow();
	scene->lights.at(0)->SpawnControlWindow();
	wnd->Gfx().RenderImgui();
	wnd->Gfx().EndFrame();
}

