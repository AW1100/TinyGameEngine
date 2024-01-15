#pragma once
#include "Core.h"
#include "Window/WindowWindows.h"
#include "Util/Timer.h"
#include "Drawable/Scene.h"

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

	private:
		WindowWindows* wnd;
		Scene* scene;
		Timer timer;
	};

	Application* CreateApplication();
}