#pragma once
#include "Drawable.h"
#include <memory>

class FullScreenPlane : public Drawable
{
public:
	FullScreenPlane(Graphics& gfx);
	~FullScreenPlane() {}
	void Update(float dt) override;
	DirectX::XMMATRIX GetModelMatrix() const override;

private:

};