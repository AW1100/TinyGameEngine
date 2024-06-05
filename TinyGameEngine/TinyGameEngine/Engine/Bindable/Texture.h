#pragma once
#include "Bindable.h"
#include <vector>

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const std::vector<const wchar_t*>& filepaths, bool& useAlpha);
	int CalculateNextTextureOffsetForMipmaping(UINT width, UINT height);
	void Bind(Graphics& gfx) override;

protected:
	std::vector<ComPtr<ID3D11ShaderResourceView>> pTextureViews;
};