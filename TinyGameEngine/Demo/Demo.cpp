#include "TinyGameEngine.h"

class Demo : public TGE::Application
{
public:
	Demo() 
	{

	}

	~Demo() {}
};

TGE::Application* TGE::CreateApplication()
{
	return new Demo();
}