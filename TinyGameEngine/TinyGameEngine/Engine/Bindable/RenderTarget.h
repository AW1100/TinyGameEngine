#pragma once
#include "GraphicsResource.h"

class RenderTarget : public GraphicsResource
{
public:
	RenderTarget(Graphics& gfx, UINT width, UINT height);
	ID3D11RenderTargetView* Get();
	void BindToOutputMerger(Graphics& gfx, ID3D11DepthStencilView* pDSV = nullptr);
	void BindToPixelShader(Graphics& gfx, UINT slot);
	void Clear(Graphics& gfx, const float* color);

private:
	ComPtr<ID3D11Texture2D> pRenderTarget;
	ComPtr<ID3D11RenderTargetView> pRTV;
	ComPtr<ID3D11ShaderResourceView> pSRV;
};