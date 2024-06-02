#include "DepthStencil.h"

DepthStencil::DepthStencil(Graphics& gfx, UINT width, UINT height, UINT numOfSamples, UINT msaaQuality)
{
	// create depth stensil texture
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = numOfSamples;
	descDepth.SampleDesc.Quality = msaaQuality;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0u;
	DX::ThrowIfFailed(GetDevice(gfx)->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	));
}

ID3D11DepthStencilView* DepthStencil::Get()
{
	return pDSV.Get();
}
