#pragma once
#include "Drawable.h"
#include "../Graphics/Graphics.h"
#include "Box.h"
#include "..\Light\PointLight.h"

class Scene
{
public:
	Scene(Graphics& gfx);
	~Scene();
	void UpdateFrame(float dt, Graphics& gfx);
	inline std::vector<PointLight*>& GetLights() { return lights; }

protected:
	std::vector<class Model*> models;
	std::vector<class Drawable*> objects;
	std::vector<class PointLight*> lights;
};