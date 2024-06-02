#pragma once
#include "Bindable.h"

class Rasterizer : public Bindable
{
public:
	Rasterizer(Graphics& gfx, bool useAlpha);
	void Bind(Graphics& gfx) override;

private:
	ComPtr<ID3D11RasterizerState> pRasterizer;
};