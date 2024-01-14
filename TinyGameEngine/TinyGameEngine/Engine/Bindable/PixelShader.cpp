#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
}

void PixelShader::Bind(Graphics& gfx)
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
