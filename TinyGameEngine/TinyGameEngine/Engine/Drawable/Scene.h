#pragma once
#include "Drawable.h"
#include "../Graphics/Graphics.h"
#include "Box.h"

class Scene
{
public:
	Scene(Graphics& gfx);
	~Scene();
	void UpdateFrame(float dt, Graphics& gfx);

private:
	std::vector<Drawable*> objects;
};