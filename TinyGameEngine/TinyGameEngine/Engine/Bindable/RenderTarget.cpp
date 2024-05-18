#include "RenderTarget.h"
#include "DepthStencil.h"

namespace wrl = Microsoft::WRL;

RenderTarget::RenderTarget(Graphics& gfx, UINT width, UINT height)
{
	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	wrl::ComPtr<ID3D11Texture2D> pTexture;
	GetDevice(gfx)->CreateTexture2D(
		&textureDesc, nullptr, &pTexture
	);

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GetDevice(gfx)->CreateShaderResourceView(
		pTexture.Get(), &srvDesc, &pTextureView
	);

	// create the target view on the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GetDevice(gfx)->CreateRenderTargetView(
		pTexture.Get(), &rtvDesc, &pTargetView
	);
}

void RenderTarget::BindAsTexture(Graphics& gfx, UINT slot) const
{
	GetContext(gfx)->PSSetShaderResources(slot, 1, pTextureView.GetAddressOf());
}

void RenderTarget::BindAsTarget(Graphics& gfx) const
{
	GetContext(gfx)->OMSetRenderTargets(1, pTargetView.GetAddressOf(), nullptr);
}

void RenderTarget::BindAsTarget(Graphics& gfx, const DepthStencil& depthStencil) const
{
	GetContext(gfx)->OMSetRenderTargets(1, pTargetView.GetAddressOf(), depthStencil.pDepthStencilView.Get());
}