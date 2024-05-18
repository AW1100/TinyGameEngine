#pragma once
#include "GraphicResource.h"

class DepthStencil;

class RenderTarget : public GraphicResource
{
public:
	RenderTarget(Graphics& gfx, UINT width, UINT height);
	void BindAsTexture(Graphics& gfx, UINT slot) const;
	void BindAsTarget(Graphics& gfx) const;
	void BindAsTarget(Graphics& gfx, const DepthStencil& depthStencil) const;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
};