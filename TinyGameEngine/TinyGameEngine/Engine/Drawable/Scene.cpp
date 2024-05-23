#include "Scene.h"
#include "Box.h"
#include "Mesh.h"
#include "Model.h"
#include "..\Util\Log.h"

#include <thread>
#include <mutex>
#include <sstream>

#include "..\Util\DynamicVertex.hpp"

#include "../Bindable/ConstantBuffer.h"


std::mutex mtx;

Scene::Scene(Graphics& gfx)
{
	CreateWorkerThread(gfx, "F:/3DModels/fbx/kafkaW.fbx", { 0.0f, -1.02f, 0.0f }, MeshType::StylishCharacterA);
	CreateWorkerThread(gfx, "F:/3DModels/obj/sponza.obj", { 0.0f, -1.0f, 0.0f }, MeshType::RealisticObjectA);

	light = std::make_shared<PointLight>(gfx, 0.03f);
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
	
	/*for (auto l : lights)
	{
		delete l;
	}*/
}

void Scene::UpdateFrame(float dt, Graphics& gfx)
{
	ShadowPass(dt, gfx);
	gfx.SetBasePassRT();
	gfx.BindShadowMapToPixelShader();
	light->Update(gfx);
	light->Draw(gfx);
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
	gfx.UnbindShadowMapToPixelShader();
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

void Scene::ShadowPass(float dt, Graphics& gfx)
{
	auto lightPosition = light->GetPosition();
	DirectX::XMVECTOR lightPos{ lightPosition.x, lightPosition.y, lightPosition.z, 0.0f };
	DirectX::XMMATRIX viewMatrices[6];
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 1.0f, 0.5f, 50.0f);
	viewMatrices[0] = DirectX::XMMatrixLookAtLH(lightPos, DirectX::XMVectorAdd(lightPos, DirectX::XMVectorSet(1, 0, 0, 0)), DirectX::XMVectorSet(0, 1, 0, 0)); // +X
	viewMatrices[1] = DirectX::XMMatrixLookAtLH(lightPos, DirectX::XMVectorAdd(lightPos, DirectX::XMVectorSet(-1, 0, 0, 0)), DirectX::XMVectorSet(0, 1, 0, 0)); // -X
	viewMatrices[2] = DirectX::XMMatrixLookAtLH(lightPos, DirectX::XMVectorAdd(lightPos, DirectX::XMVectorSet(0, 1, 0, 0)), DirectX::XMVectorSet(0, 0, -1, 0)); // +Y
	viewMatrices[3] = DirectX::XMMatrixLookAtLH(lightPos, DirectX::XMVectorAdd(lightPos, DirectX::XMVectorSet(0, -1, 0, 0)), DirectX::XMVectorSet(0, 0, 1, 0)); // -Y
	viewMatrices[4] = DirectX::XMMatrixLookAtLH(lightPos, DirectX::XMVectorAdd(lightPos, DirectX::XMVectorSet(0, 0, 1, 0)), DirectX::XMVectorSet(0, 1, 0, 0)); // +Z
	viewMatrices[5] = DirectX::XMMatrixLookAtLH(lightPos, DirectX::XMVectorAdd(lightPos, DirectX::XMVectorSet(0, 0, -1, 0)), DirectX::XMVectorSet(0, 1, 0, 0)); // -Z

	

	for (int i = 0; i < 6; ++i)
	{
		gfx.SetShadowPassRT(i);
		for (auto& object : objects)
		{
			struct VSCBuf
			{
				DirectX::XMMATRIX model;
				DirectX::XMMATRIX MVP;
			};
			VSCBuf vb;
			vb.model = DirectX::XMMatrixTranspose(object->GetModelMatrix());
			vb.MVP = DirectX::XMMatrixTranspose(object->GetModelMatrix() * viewMatrices[i] * projMatrix);
			VertexConstantBuffer<VSCBuf> vcb(gfx, vb);
			vcb.slot = 0;
			vcb.Bind(gfx);

			Mesh* temp = dynamic_cast<Mesh*>(object);
			if (temp)
			{
				temp->DrawShadowMap(gfx);
			}
		}
		gfx.ClearRenderTarget();
	}
}
