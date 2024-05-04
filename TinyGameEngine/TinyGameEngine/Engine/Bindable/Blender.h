#pragma once
#include "Bindable.h"

class Blender : public Bindable
{
public:
	Blender(Graphics& gfx, bool blending);
	void Bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
	bool blending;
};