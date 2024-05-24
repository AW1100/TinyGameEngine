#include "ShadowRasterizer.h"
#include "../Exception/Exception.h"

ShadowRasterizer::ShadowRasterizer(Graphics& gfx, int depthBias, float slopeBias, float clamp)
{
	ChangeDepthBiasParameters(gfx, depthBias, slopeBias, clamp);
}

void ShadowRasterizer::ChangeDepthBiasParameters(Graphics& gfx, int depthBias, float slopeBias, float clamp)
{
	this->depthBias = depthBias;
	this->slopeBias = slopeBias;
	this->clamp = clamp;

	D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterDesc.DepthBias = depthBias;
	rasterDesc.SlopeScaledDepthBias = slopeBias;
	rasterDesc.DepthBiasClamp = clamp;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
}

void ShadowRasterizer::Bind(Graphics& gfx)
{
	GetContext(gfx)->RSSetState(pRasterizer.Get());
}
