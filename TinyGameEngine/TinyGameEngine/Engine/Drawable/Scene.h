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

	std::vector<std::unique_ptr<PointLight>> lights;
protected:
	std::vector<std::shared_ptr<Drawable>> objects;
};