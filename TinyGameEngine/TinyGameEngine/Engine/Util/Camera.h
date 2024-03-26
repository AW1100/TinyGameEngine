#pragma once
#include "../Graphics/Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetView() const;
	void SpawnControlWindow();
	void Reset();

private:
	float r = 2.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};