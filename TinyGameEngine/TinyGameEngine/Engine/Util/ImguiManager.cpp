#include "ImguiManager.h"
#include "imgui.h"

ImguiManager::ImguiManager()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}
