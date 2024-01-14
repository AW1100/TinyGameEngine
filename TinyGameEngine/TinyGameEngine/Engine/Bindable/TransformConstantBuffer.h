#pragma once
#include "ConstantBuffer.h"
#include "../Drawable/Drawable.h"

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) override;

private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;
};