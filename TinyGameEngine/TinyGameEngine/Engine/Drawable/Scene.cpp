#include "Scene.h"
#include "Box.h"
#include "Mesh.h"
#include "Model.h"


#include <sstream>

Scene::Scene(Graphics& gfx)
{
	models.push_back(new Model("F:/3DModels/fbx/kafkaW.fbx", { 0.0f, -1.0f, 0.0f }));
	models.push_back(new Model("F:/3DModels/obj/sponza.obj", { 0.0f, -1.0f, 0.0f }));

	for (auto& m : models)
	{
		m->FindRenderables(objects, gfx);
	}

	lights.push_back(new PointLight(gfx, 0.03f));
}

Scene::~Scene()
{
	for (auto o : objects)
	{
		delete o;
	}

	for (auto m : models)
	{
		delete m;
	}
	
	for (auto l : lights)
	{
		delete l;
	}
}

void Scene::UpdateFrame(float dt, Graphics& gfx)
{
	for (auto& light : lights)
	{
		light->Update(gfx);
		light->Draw(gfx);
	}
	for (auto& obj : objects)
	{
		obj->Update(dt);
		obj->Draw(gfx);
	}
}
