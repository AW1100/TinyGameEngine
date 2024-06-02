#pragma once
#include "GraphicsResource.h"

class DepthCubemap : public GraphicsResource
{
public:
	DepthCubemap(Graphics& gfx, UINT size);
	ID3D11DepthStencilView* Get();

private:
	ComPtr<ID3D11Texture2D> pShadowDepth;
	ComPtr<ID3D11DepthStencilView> pShadowCubeDSV[6];
	ComPtr<ID3D11ShaderResourceView> pShadowDepthSRV;
};