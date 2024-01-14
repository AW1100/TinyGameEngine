#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent)
	:vcbuf(gfx),parent(parent)
{
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	vcbuf.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
	vcbuf.Bind(gfx);
}
