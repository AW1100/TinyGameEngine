#pragma once
#include "Core.h"
#include "Window/WindowWindows.h"
#include "Util/Timer.h"
#include "Drawable/Scene.h"
#include "Util\ImguiManager.h"
#include "Util\Camera.h"

namespace TGE
{
	class TGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		int Run();
	private:
		void DoFrame();

	protected:
		Scene* scene;
	private:
		WindowWindows* wnd;
		Timer timer;
		ImguiManager im;
		Camera cam;

		float speed_factor = 1.0f;
	};

	Application* CreateApplication();
}