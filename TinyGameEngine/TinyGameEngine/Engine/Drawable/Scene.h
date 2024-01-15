#pragma once
#include "Drawable.h"
#include "../Graphics/Graphics.h"

class Scene
{
public:
	Scene(Graphics& gfx);
	~Scene() = default;
	void UpdateFrame(float dt, Graphics& gfx);

private:
	std::vector<std::unique_ptr<Drawable>> objects;
};