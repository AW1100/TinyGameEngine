#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent)
	:parent(parent)
{
	if (!vcbuf)
	{
		vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	vcbuf->Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection()));
	vcbuf->Bind(gfx);
}
