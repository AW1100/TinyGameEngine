#pragma once
#include "Drawable.h"
#include "../Util/ShaderLookupTable.h"

#include <random>

class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, std::shared_ptr<class MeshNode> node, DirectX::XMFLOAT3 trans, MeshType type, const std::string& n, unsigned int vertexType, bool useOutline);
	virtual ~Mesh() override;
	void Update(float dt) override;
	DirectX::XMMATRIX GetModelMatrix() const override;
	void DrawOutline(Graphics& gfx);
	inline bool IsOutlineEnabled() { return outlineEnabled; }
	void DrawShadowMap(Graphics& gfx);
	virtual bool IsTranslucent() const override { return useAlpha; }
	virtual bool operator<(const Drawable& other) const override { return useAlpha < other.IsTranslucent(); }

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
	bool outlineEnabled = false;
	bool outlining = false;
	bool useAlpha = false;
	std::vector<std::shared_ptr<Bindable>> outlineBinds;
	std::vector<std::shared_ptr<Bindable>> shadowBinds;
};