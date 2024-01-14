#pragma once
#include "Bindable.h"

#include <string>

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) override;

protected:
	ComPtr<ID3D11PixelShader> pPixelShader;
};