#include "Application.h"
#include "../Math/Math.h"
#include "Event/Event.h"

#include <iostream>

TGE::Application::Application()
{
	wnd = new WindowWindows(800, 450);
	wnd->Init("Tiny Game Engine");
	wnd->Show();

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 10; i++)
	{
		boxes.push_back(new Box(
			wnd->Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd->Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

TGE::Application::~Application()
{
	/*for (auto& b : boxes)
	{
		delete b;
	}*/
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
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd->Gfx());
	}
	wnd->Gfx().EndFrame();
}

