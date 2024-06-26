//#pragma once
//#include "DrawableBase.h"
//
//#include <random>
//
//class Box : public DrawableBase<Box>
//{
//public:
//	Box(Graphics& gfx,std::mt19937& rng,
//		std::uniform_real_distribution<float>& adist,
//		std::uniform_real_distribution<float>& ddist,
//		std::uniform_real_distribution<float>& odist,
//		std::uniform_real_distribution<float>& rdist);
//	virtual ~Box() override;
//	void Update(float dt) override;
//	DirectX::XMMATRIX GetModelMatrix() const override;
//
//private:
//	float r;
//	float roll = 0.0f;
//	float pitch = 0.0f;
//	float yaw = 0.0f;
//	float theta;
//	float phi;
//	float chi;
//
//	float droll;
//	float dpitch;
//	float dyaw;
//	float dtheta;
//	float dphi;
//	float dchi;
//};