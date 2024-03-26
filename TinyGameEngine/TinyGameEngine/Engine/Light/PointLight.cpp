#include "PointLight.h"
#include "imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:cbuf(gfx)
{
	pos = { 0.0f,1.0f,0.0f };
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
		ImGui::SliderFloat("X", &pos.x, -5.0f, 5.0f, "%.3f");
		ImGui::SliderFloat("Y", &pos.y, -5.0f, 5.0f, "%.3f");
		ImGui::SliderFloat("Z", &pos.z, -5.0f, 5.0f, "%.3f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	pos = { 0.0f,1.0f,0.0f };
}

void PointLight::Draw(Graphics& gfx)
{
	mesh->SetPos(pos);
	mesh->Draw(gfx);
}

void PointLight::Update(Graphics& gfx)
{
	cbuf.Update(gfx, PointLightCBuf{ pos });
	cbuf.Bind(gfx);
}


