#pragma once

#ifdef TGE_PLATFORM_WINDOWS

TGE::Application* TGE::CreateApplication();

int main(int argc, char** argv)
{
	auto app = TGE::CreateApplication();
	app->Run();
	delete app;
}

#endif