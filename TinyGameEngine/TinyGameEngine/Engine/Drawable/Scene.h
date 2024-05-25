#pragma once
#include "Drawable.h"
#include "../Graphics/Graphics.h"
#include "Box.h"
#include "..\Light\PointLight.h"

#include <thread>

enum class MeshType : int;

class Scene
{
public:
	Scene(Graphics& gfx);
	~Scene();
	void UpdateFrame(float dt, Graphics& gfx);
	inline std::shared_ptr<PointLight>& GetLights() { return light; }
	void CreateWorkerThread(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type, bool outline = false);
	void LoadMeshAsync(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type, bool outline = false);
	void ShadowPass(float dt, Graphics& gfx);
	void BasePass(float dt, Graphics& gfx);
	void PostPass(float dt, Graphics& gfx);

protected:
	std::vector<class Model*> models;
	std::vector<class Drawable*> objects;
	std::vector<class Drawable*> loadedObjects;
	std::shared_ptr<class PointLight> light;
};