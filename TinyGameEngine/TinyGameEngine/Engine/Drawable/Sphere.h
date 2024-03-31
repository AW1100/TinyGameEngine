#pragma once
#include "DrawableBase.h"

#include <random>

class Sphere : public DrawableBase<Sphere>
{
public:
	Sphere(Graphics& gfx, float radius);
	virtual ~Sphere() override;
	void SetPos(DirectX::XMFLOAT4 p);
	void Update(float dt) override;
	DirectX::XMMATRIX GetModelMatrix() const override;
	void createSphere(float radius, int numLatitudeLines, int numLongitudeLines,
		std::vector<DirectX::XMFLOAT3>& vertices, std::vector<unsigned int>& indices);

private:
	DirectX::XMFLOAT4 pos;
};