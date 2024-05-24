#pragma once
#include "Bindable.h"

class ShadowRasterizer : public Bindable
{
public:
	ShadowRasterizer(Graphics& gfx, int depthBias, float slopeBias, float clamp);
	void ChangeDepthBiasParameters(Graphics& gfx, int depthBias, float slopeBias, float clamp);
	void Bind(Graphics& gfx) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;

public:
	int depthBias;
	float slopeBias;
	float clamp;
};