#pragma once
#include "Drawable.h"

class Skybox : public Drawable
{
public:
	Skybox(Graphics& gfx);
	virtual ~Skybox();
	virtual void Update(float dt) override {}
	DirectX::XMMATRIX GetModelMatrix() const override;

private:

};