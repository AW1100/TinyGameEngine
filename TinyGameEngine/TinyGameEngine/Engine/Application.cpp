#include "Application.h"
#include "../Math/Math.h"
#include "Event/Event.h"
#include "Constants.h"
#include "Light\PointLight.h"

#include <iostream>
#include <d3d11sdklayers.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

TGE::Application::Application()
{
	wnd = new WindowWindows(WINDOW_WIDTH, WINDOW_HEIGHT);
	wnd->Init(WINDOW_TITLE);
	wnd->Show();
	scene = new Scene(wnd->Gfx());
	wnd->Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, WINDOW_HEIGHT/WINDOW_WIDTH, NEAR_PLANE, FAR_PLANE));
	cam.SetConstantBuffer(wnd->Gfx());
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
	cam.Update(wnd->Gfx(),dt);
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << timer.Duration();
	SetWindowText(wnd->GetHWnd(), WINDOW_TITLE);
	wnd->Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	scene->UpdateFrame(dt, wnd->Gfx());
	wnd->Gfx().BeginImgui(speed_factor);
	Log::GetInstance().RenderLogWindow();
	cam.SpawnControlWindow();
	scene->GetLights()->SpawnControlWindow();
	wnd->Gfx().RenderImgui();
	wnd->Gfx().EndFrame();
}

