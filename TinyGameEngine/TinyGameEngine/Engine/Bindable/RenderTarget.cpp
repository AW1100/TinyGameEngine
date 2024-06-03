#include "RenderTarget.h"

RenderTarget::RenderTarget(Graphics& gfx, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = 0;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &pRenderTarget));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2DArray.MipSlice = 0;
	rtvDesc.Texture2DArray.ArraySize = 1;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateRenderTargetView(pRenderTarget.Get(), &rtvDesc, &pRTV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = 1;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(pRenderTarget.Get(), &srvDesc, &pSRV));
}

ID3D11RenderTargetView* RenderTarget::Get()
{
	return pRTV.Get();
}

void RenderTarget::BindToOutputMerger(Graphics& gfx, ID3D11DepthStencilView* pDSV)
{
	GetContext(gfx)->OMSetRenderTargets(1u, pRTV.GetAddressOf(), pDSV);
}

void RenderTarget::BindToPixelShader(Graphics& gfx, UINT slot)
{
	GetContext(gfx)->PSSetShaderResources(slot, 1, pSRV.GetAddressOf());
}

void RenderTarget::Clear(Graphics& gfx, const float* color)
{
	GetContext(gfx)->ClearRenderTargetView(pRTV.Get(), color);
}
