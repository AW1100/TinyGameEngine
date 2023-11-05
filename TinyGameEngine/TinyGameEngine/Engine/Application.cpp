#include "Application.h"
#include "../Math/Math.h"
#include <iostream>

TGE::Application::Application()
{
}

TGE::Application::~Application()
{
}

void TGE::Application::Run()
{
	FMatrix4 m;
	FMatrix3 m2;
	FVector3 v1;
	FVector4 v2;
	while (true)
	{
		std::cout << m.m[3][3] << std::endl;
	}
}

