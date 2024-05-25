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
	inline DirectX::XMFLOAT4& GetPosition() { return pos; }
	inline std::shared_ptr<Sphere> GetMesh() { return mesh; }
private:
	struct alignas(16) PointLightCBuf
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 attConsts;
	};
private:
	DirectX::XMFLOAT4 initialPos = { 2.0f,1.0f, -1.0f,1.0f };
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 attConsts = { 0.7f, 0.3f, 0.2f, 30.0f }; // attConst, attLin, attQuad, alpha
	mutable std::shared_ptr<Sphere> mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};