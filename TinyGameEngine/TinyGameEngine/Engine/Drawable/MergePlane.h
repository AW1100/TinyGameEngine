#pragma once
#include "Drawable.h"
#include <memory>

class MergePlane : public Drawable
{
public:
	MergePlane(Graphics& gfx);
	~MergePlane() {}
	void Update(float dt) override;
	DirectX::XMMATRIX GetModelMatrix() const override;

private:

};