#pragma once
#include "DrawableBase.h"

#include <random>

class Model : public DrawableBase<Model>
{
public:
	Model(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	virtual ~Model() override;
	void Update(float dt) override;
	DirectX::XMMATRIX GetTransformXM() const override;
	void loadModel();

private:
	float r = 0.0f;
	float roll = 0.0f;
	float pitch = -90.0f;
	float yaw = 0.0f;
	float theta = 90.0f;
	float phi = 0.0f;
	float chi = 0.0f;

	float droll = 0.0f;
	float dpitch = 0.0f;
	float dyaw = 0.0f;
	float dtheta = 0.0f;
	float dphi = 0.0f;
	float dchi = 0.0f;
	class aiMesh* pMesh;
};