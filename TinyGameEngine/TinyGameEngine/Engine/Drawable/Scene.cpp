#include "Scene.h"
#include "Box.h"

#include <sstream>

Scene::Scene(Graphics& gfx)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 10; i++)
	{
		objects.push_back(std::make_unique<Box>(
			gfx, rng, adist,
			ddist, odist, rdist
		));
	}
}

Scene::~Scene()
{

}

void Scene::UpdateFrame(float dt, Graphics& gfx)
{
	for (auto& obj : objects)
	{
		obj->Update(dt);
		obj->Draw(gfx);
	}
}
