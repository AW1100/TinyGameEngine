#pragma once
#include "Core.h"

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
	};

	Application* CreateApplication();
}