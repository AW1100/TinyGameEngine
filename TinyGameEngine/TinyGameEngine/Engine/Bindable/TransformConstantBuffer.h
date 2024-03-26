#pragma once
#include "ConstantBuffer.h"
#include "../Drawable/Drawable.h"
#include <memory>

class TransformConstantBuffer : public Bindable
{
	struct Transform
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX modelViewProj;
	};

public:
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) override;

private:
	inline static std::unique_ptr<VertexConstantBuffer<Transform>> vcbuf;
	const Drawable& parent;
};