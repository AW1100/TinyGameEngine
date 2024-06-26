#pragma once
#include "../Graphics/Graphics.h"
#include "..\Bindable\ConstantBuffer.h"
#include <memory>

class Camera
{
public:
	Camera(float r = 3.0f, float theta = -1.0f, float phi = 0.0f, float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f);
	DirectX::XMMATRIX GetView() const;
	void SetConstantBuffer(Graphics& gfx);
	void Update(Graphics& gfx, float dt);
	void SpawnControlWindow();
	void Reset();

private:
	float r;
	float theta;
	float phi;
	float pitch;
	float yaw;
	float roll;
	mutable DirectX::XMVECTOR eyePos;
	std::unique_ptr<PixelConstantBuffer<DirectX::XMVECTOR>> pcb;
};