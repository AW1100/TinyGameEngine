#pragma once
#include "GraphicsResource.h"

class DepthCubemap : public GraphicsResource
{
public:
	DepthCubemap(Graphics& gfx, UINT size);
	ID3D11DepthStencilView* Get(int index);
	void BindToOutputMerger(Graphics& gfx);
	void BindToPixelShader(Graphics& gfx, UINT slot);
	void Clear(Graphics& gfx);

private:
	ComPtr<ID3D11Texture2D> pShadowDepth;
	ComPtr<ID3D11DepthStencilView> pShadowCubeDSV[6];
	ComPtr<ID3D11ShaderResourceView> pShadowDepthSRV;
};