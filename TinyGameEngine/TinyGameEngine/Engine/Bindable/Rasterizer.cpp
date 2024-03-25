#include "Rasterizer.h"

Rasterizer::Rasterizer(Graphics& gfx)
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.FillMode = D3D11_FILL_SOLID;  // Or D3D11_FILL_WIREFRAME to render in wireframe mode
	rasterDesc.CullMode = D3D11_CULL_NONE;    // Turn off back-face culling
	rasterDesc.FrontCounterClockwise = false; // This is the default value, but you can set it as needed
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.MultisampleEnable = false;  // Set to true if you're using multisampling
	rasterDesc.AntialiasedLineEnable = false; // Set to true to enable line antialiasing

	HRESULT result = GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer);

	if (FAILED(result))
	{
		// Handle error
	}
}

void Rasterizer::Bind(Graphics& gfx)
{
	GetContext(gfx)->RSSetState(pRasterizer.Get());
}
