#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& gfx) override;

private:
	D3D11_PRIMITIVE_TOPOLOGY type;
};