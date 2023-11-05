#pragma once

#ifdef TGE_PLATFORM_WINDOWS

extern TGE::Application* TGE::CreateApplication();

int main(int argc, char** argv)
{
	auto app = TGE::CreateApplication();
	app->Run();
	delete app;
}

#endif