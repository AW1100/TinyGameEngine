#pragma once
#include "Drawable.h"

#include <random>

enum class MeshType
{
	Unknown = 0,
	Anima_Character = 1,
	Scene = 2
};

class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, std::shared_ptr<class MeshNode> node, DirectX::XMFLOAT3 trans, MeshType type, const std::string& n);
	virtual ~Mesh() override;
	void Update(float dt) override;
	DirectX::XMMATRIX GetModelMatrix() const override;

private:
	float r = 0.0f;
	float roll = 0.0f;
	float pitch = 90.0f;
	float yaw = 0.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float chi = 0.0f;

	float droll = 0.0f;
	float dpitch = 0.0f;
	float dyaw = 0.0f;
	float dtheta = 0.0f;
	float dphi = 0.0f;
	float dchi = 0.0f;

	DirectX::XMFLOAT3 translation;
};