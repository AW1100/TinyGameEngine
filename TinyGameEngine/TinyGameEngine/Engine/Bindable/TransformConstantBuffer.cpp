#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent)
	:parent(parent)
{
	if (!vcbuf)
	{
		vcbuf = std::make_unique<VertexConstantBuffer<Transform>>(gfx);
	}
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	const auto model = parent.GetModelMatrix();
	const Transform tf =
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	};
	vcbuf->Update(gfx, tf);
	vcbuf->Bind(gfx);
}
