#pragma once
#include "ConstantBuffer.h"
#include "../Drawable/Drawable.h"
#include <memory>

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) override;

private:
	inline static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcbuf;
	const Drawable& parent;
};