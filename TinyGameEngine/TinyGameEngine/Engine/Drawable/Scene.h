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
	inline std::vector<PointLight*>& GetLights() { return lights; }
	void CreateWorkerThread(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type);
	void LoadMeshAsync(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type);

protected:
	std::vector<class Model*> models;
	std::vector<class Drawable*> objects;
	std::vector<class Drawable*> loadedObjects;
	std::vector<class PointLight*> lights;
};