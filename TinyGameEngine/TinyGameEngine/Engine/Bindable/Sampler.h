#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics& gfx);
	Sampler(Graphics& gfx, UINT slot);
	void Bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	UINT slot = 0u;
};