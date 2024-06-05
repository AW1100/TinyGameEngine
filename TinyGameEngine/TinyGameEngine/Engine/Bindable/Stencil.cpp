#include "Stencil.h"

Stencil::Stencil(Graphics& gfx, Mode mode)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	if (mode == Mode::Write)
	{
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilWriteMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if (mode == Mode::Mask)
	{
		dsDesc.DepthEnable = FALSE;
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}
	else if (mode == Mode::DepthLessEqual)
	{
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}

	GetDevice(gfx)->CreateDepthStencilState(&dsDesc, &pStencil);
}

void Stencil::Bind(Graphics& gfx)
{
	GetContext(gfx)->OMSetDepthStencilState(pStencil.Get(), 0xFF);
}
