#include "Scene.h"
#include "Box.h"
#include "Mesh.h"
#include "Model.h"
#include "..\Util\Log.h"

#include <thread>
#include <mutex>
#include <sstream>

#include "..\Util\DynamicVertex.hpp"


std::mutex mtx;

Scene::Scene(Graphics& gfx)
{
	CreateWorkerThread(gfx, "F:/3DModels/fbx/kafkaW.fbx", { 0.0f, -1.0f, 0.0f }, MeshType::StylishCharacterA, true);
	CreateWorkerThread(gfx, "F:/3DModels/obj/sponza.obj", { 0.0f, -1.0f, 0.0f }, MeshType::RealisticObjectA);

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
	mtx.lock();
	if (loadedObjects.size() > 0)
	{
		objects.insert(objects.end(), loadedObjects.begin(), loadedObjects.end());
		loadedObjects.clear();
	}
	for (auto& obj : objects)
	{
		obj->Update(dt);
		obj->Draw(gfx);
	}
	for (auto& obj : objects)
	{
		Mesh* temp = dynamic_cast<Mesh*>(obj);
		if (temp && temp->IsOutlineEnabled())
		{
			temp->Update(dt);
			temp->DrawOutline(gfx);
		}	
	}
	mtx.unlock();
}

void Scene::CreateWorkerThread(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type, bool outline)
{
	std::thread worker(&Scene::LoadMeshAsync, this, std::ref(gfx), filename, trans, type, outline);
	worker.detach();
}

void Scene::LoadMeshAsync(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type, bool outline)
{
	//Log::GetInstance().AddLog((std::string("Loading ")+std::string(filename)).c_str());
	Model* temp = new Model(filename, trans, type);
	temp->UseOutline(outline);
	models.push_back(temp);
	//Log::GetInstance().AddLog("Load Complete");
	mtx.lock();
	temp->FindRenderables(loadedObjects, gfx);
	mtx.unlock();
}
