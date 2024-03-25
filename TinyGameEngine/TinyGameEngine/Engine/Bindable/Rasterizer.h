#pragma once
#include "Bindable.h"

class Rasterizer : public Bindable
{
public:
	Rasterizer(Graphics& gfx);
	void Bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
};