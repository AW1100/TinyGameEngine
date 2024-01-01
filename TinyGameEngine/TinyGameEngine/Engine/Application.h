#pragma once
#include "Core.h"
#include "Window/WindowWindows.h"

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
	};

	Application* CreateApplication();
}