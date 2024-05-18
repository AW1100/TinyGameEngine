#pragma once
#include "GraphicResource.h"

class Bindable : public GraphicResource
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	virtual ~Bindable() = default;

};