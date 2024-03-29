#pragma once
#include "../Graphics/Graphics.h"

class Camera
{
public:
	Camera(float r = 2.0f, float theta = -0.5f, float phi = 0.0f, float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f);
	DirectX::XMMATRIX GetView() const;
	void SpawnControlWindow();
	void Reset();

private:
	float r;
	float theta;
	float phi;
	float pitch;
	float yaw;
	float roll;
};