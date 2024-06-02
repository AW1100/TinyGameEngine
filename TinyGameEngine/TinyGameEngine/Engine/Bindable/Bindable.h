#pragma once
#include "GraphicsResource.h"

class Bindable : public GraphicsResource
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	virtual ~Bindable() = default;
};