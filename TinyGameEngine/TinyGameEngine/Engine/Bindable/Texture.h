#pragma once
#include "Bindable.h"
#include <vector>

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const wchar_t* filepath);
	Texture(Graphics& gfx, const std::vector<const wchar_t*>& filepaths);
	void Bind(Graphics& gfx) override;

protected:
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> pTextureViews;
};