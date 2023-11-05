#include "Application.h"
#include <iostream>

TGE::Application::Application()
{
}

TGE::Application::~Application()
{
}

void TGE::Application::Run()
{
	while (true)
	{
		std::cout << "hello world" << std::endl;
	}
}

