#pragma once
#include "..\Graphics\Graphics.h"
#include "..\Drawable\Sphere.h"
#include "..\Bindable\ConstantBuffer.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius);
	~PointLight();
	void SpawnControlWindow();
	void Reset();
	void Draw(Graphics& gfx);
	void Update(Graphics& gfx);
	inline std::shared_ptr<Sphere> GetMesh() { return mesh; }
private:
	struct alignas(16) PointLightCBuf
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color;
	};
private:
	DirectX::XMFLOAT4 initialPos = { 1.0f,1.0f, 2.0f,1.0f };
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	mutable std::shared_ptr<Sphere> mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};