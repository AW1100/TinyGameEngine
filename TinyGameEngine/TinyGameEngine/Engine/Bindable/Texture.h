#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const wchar_t* filepath);
	void Bind(Graphics& gfx) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};