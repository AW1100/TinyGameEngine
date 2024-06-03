#include "DepthCubemap.h"

DepthCubemap::DepthCubemap(Graphics& gfx, UINT size)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = size;
	texDesc.Height = size;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&texDesc, nullptr, &pShadowDepth));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 1;


	for (int i = 0; i < 6; ++i)
	{
		dsvDesc.Texture2DArray.FirstArraySlice = i;
		DX::ThrowIfFailed(GetDevice(gfx)->CreateDepthStencilView(pShadowDepth.Get(), &dsvDesc, &pShadowCubeDSV[i]));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = 1;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(pShadowDepth.Get(), &srvDesc, &pShadowDepthSRV));
}

ID3D11DepthStencilView* DepthCubemap::Get(int index)
{
	return pShadowCubeDSV[index].Get();
}

void DepthCubemap::BindToOutputMerger(Graphics& gfx)
{
}

void DepthCubemap::BindToPixelShader(Graphics& gfx, UINT slot)
{
	GetContext(gfx)->PSSetShaderResources(slot, 1, pShadowDepthSRV.GetAddressOf());
}

void DepthCubemap::Clear(Graphics& gfx)
{
	for (int i = 0; i < 6; i++)
	{
		GetContext(gfx)->ClearDepthStencilView(pShadowCubeDSV[i].Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}
}
