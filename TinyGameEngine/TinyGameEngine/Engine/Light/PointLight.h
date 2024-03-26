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
		DirectX::XMFLOAT3 pos;
	};
private:
	DirectX::XMFLOAT3 pos;
	mutable std::shared_ptr<Sphere> mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};