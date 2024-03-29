#include "Log.h"
#include "..\ThirdParty\imgui\imgui.h"

#include <fstream>

void Log::AppendStringToFile()
{
    std::ofstream file(CONF_PATH, std::ios::app);

    if (!file.is_open()) {
        return;
    }

    // Append the string to the file
    file << logBuffer;

    // Optionally add a newline if needed
    //file << "\n";

    logBuffer.clear();
}

std::string Log::ReadFileContent(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "Failed to open file: " + filePath;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Log::RenderLogWindow()
{
    ImGui::Begin("Log");

    // Create a child ImGui window to hold the text and make it scrollable
    ImGui::BeginChild("Scrolling");

    std::string fileContent = ReadFileContent(CONF_PATH);
    ImGui::TextUnformatted(fileContent.c_str());
    // Scroll to the bottom if the log is updated
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    
    ImGui::EndChild();

    if (ImGui::Button("Clear"))
    {
        Clear();
    }
    ImGui::End();
}

void Log::Clear()
{
    std::ofstream file(CONF_PATH, std::ios::trunc);

    if (!file.is_open()) {
        return;
    }
}
