#pragma once
#include "GraphicsResource.h"

class DepthStencil : public GraphicsResource
{
public:
	DepthStencil(Graphics& gfx, UINT width, UINT height, UINT numOfSamples, UINT msaaQuality);
	ID3D11DepthStencilView* Get();

private:
	ComPtr<ID3D11DepthStencilView> pDSV;
};