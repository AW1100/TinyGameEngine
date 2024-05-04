#include "PointLight.h"
#include "imgui.h"
#include "..\Util\Log.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:cbuf(gfx,0)
{
	pos = initialPos;
	mesh = std::make_shared<Sphere>(gfx, radius);
}

PointLight::~PointLight()
{
}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -15.0f, 15.0f, "%.3f");
		ImGui::SliderFloat("Y", &pos.y, -15.0f, 15.0f, "%.3f");
		ImGui::SliderFloat("Z", &pos.z, -15.0f, 15.0f, "%.3f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	pos = initialPos;
	LOG("Point Light position reset.")
}

void PointLight::Draw(Graphics& gfx)
{
	mesh->SetPos(pos);
	mesh->Draw(gfx);
}

void PointLight::Update(Graphics& gfx)
{
	cbuf.Update(gfx, PointLightCBuf{ pos,color });
	cbuf.Bind(gfx);
}


