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
	/*auto LoadModels = [&]() -> void {
		Log::GetInstance().AddLog("Loading meshes ...");
		models.push_back(new Model("F:/3DModels/fbx/kafkaW.fbx", { 0.0f, -1.0f, 0.0f }, MeshType::Anima_Character));
		models.push_back(new Model("F:/3DModels/obj/sponza.obj", { 0.0f, -1.0f, 0.0f }, MeshType::Scene));
		Log::GetInstance().AddLog("Load Complete");
		mtx.lock();
		for (auto& m : models)
		{
			m->FindRenderables(objects, gfx);
		}
		mtx.unlock();
		};

	std::thread LoadWorker(LoadModels);
	LoadWorker.detach();*/
	/*struct Test
	{
		float x;
		float y;
		float z;
	};
	Test t0;
	t0.x = 0.5;
	t0.y = 1.5;
	t0.z = 2.5;
	VertexLayout v;
	v.Append({ ElementType::Position3D });
	v.Append({ ElementType::Texture2D });
	v.Append({ ElementType::Normal });
	DynamicVertex dv(std::move(v));
	dv.Store(t0);
	for (size_t i = 0; i < dv.contents.size(); i += 4)
	{
		float value;
		std::memcpy(&value, &dv.contents[i], sizeof(float));
		int pause = 0;

	}*/

	CreateWorkerThread(gfx, "F:/3DModels/fbx/kafkaW.fbx", { 0.0f, -1.0f, 0.0f }, MeshType::Anima_Character);
	CreateWorkerThread(gfx, "F:/3DModels/obj/sponza.obj", { 0.0f, -1.0f, 0.0f }, MeshType::Scene);

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
	mtx.unlock();
}

void Scene::CreateWorkerThread(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type)
{
	std::thread worker(&Scene::LoadMeshAsync, this, std::ref(gfx), filename, trans, type);
	worker.detach();
}

void Scene::LoadMeshAsync(Graphics& gfx, const char* filename, DirectX::XMFLOAT3 trans, MeshType type)
{
	//Log::GetInstance().AddLog((std::string("Loading ")+std::string(filename)).c_str());
	Model* temp = new Model(filename, trans, type);
	models.push_back(temp);
	//Log::GetInstance().AddLog("Load Complete");
	mtx.lock();
	temp->FindRenderables(loadedObjects, gfx);
	mtx.unlock();
}
