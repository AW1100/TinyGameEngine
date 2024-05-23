#include "GeometryShader.h"

GeometryShader::GeometryShader(Graphics& gfx, const std::wstring& path)
{
	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	GetDevice(gfx)->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pGeometryShader);
}

void GeometryShader::Bind(Graphics& gfx)
{
	GetContext(gfx)->GSSetShader(pGeometryShader.Get(), nullptr, 0u);
}
