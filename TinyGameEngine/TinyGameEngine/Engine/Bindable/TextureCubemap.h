#pragma once
#include "Bindable.h"

#include <vector>

class TextureCubemap : public Bindable
{
public:
	TextureCubemap(Graphics& gfx, const std::vector<const wchar_t*>& filepaths, UINT slot = 0u);
	virtual ~TextureCubemap();
	void Bind(Graphics& gfx) override;

private:
	std::vector<ComPtr<ID3D11ShaderResourceView>> pTextureViews;
	UINT slot;
};