#pragma once
#include "Bindable.h"

class Stencil : public Bindable
{
public:
	enum class Mode
	{
		Off,
		Write,
		Mask,
		DepthLessEqual
	};

	Stencil(Graphics& gfx, Mode mode);

	void Bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pStencil;
};