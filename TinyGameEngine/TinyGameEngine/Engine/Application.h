#pragma once
#include "Core.h"
#include "Window/WindowWindows.h"
#include "Util/Timer.h"
#include "Drawable/Box.h"

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
		std::vector<Box*> boxes;
		Timer timer;
	};

	Application* CreateApplication();
}