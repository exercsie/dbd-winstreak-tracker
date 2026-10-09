#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "Gui.hpp"
#include "Tracker.hpp"

#include <cstdint>          // std::uint32_t
#include <format>           // std::format

bool GUI::buttonColour(const char* name, ImVec4 v, ImVec2 size) {
    ImGui::PushStyleColor(ImGuiCol_Button, v);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(v.x + 0.1f, v.y + 0.1f, v.z + 0.1f, 1.0f)); // add +.1 on hover
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(v.x - 0.1f, v.y - 0.1f, v.z - 0.1f, 1.0f)); // decrement -.1 on press to simulate diff states
    const bool isButtonPressed = ImGui::Button(name, size);
    ImGui::PopStyleColor(3);
    return isButtonPressed;
}

void GUI::displayAllKillerStats(const Tracker& t) const noexcept {
    for(const auto& [killer, data] : t.getMap()) {
        ImGui::Separator();
        ImGui::Text(std::format("Killer: {}\nWins: {}\nPB: {}", killer.c_str(), data.wins, data.personalBest).c_str());
    }
}

void GUI::displayKillerWinstreaksInReferenceToN(const Tracker& t, const std::uint32_t n) const noexcept {
    bool notFound = true;
    for(const auto& [killer, data] : t.getMap()) {
        if(data.wins >= n) {
            ImGui::Separator();
            ImGui::Text(std::format("Killer: {}\nWins: {}", killer.c_str(), data.wins).c_str());
            notFound = false;
        }
    }

    if(notFound) {
        ImGui::Separator();
        ImGui::Text("No results found!");
    }
} 

void GUI::displayKillerPersonalBestsInReferenceToN(const Tracker& t, const std::uint32_t n) const noexcept {
    bool notFound = true;
    for(const auto& [killer, data] : t.getMap()) {
        if(data.personalBest >= n) {
            ImGui::Separator();
            ImGui::Text(std::format("Killer: {}\nPB: {}", killer.c_str(), data.personalBest).c_str());
            notFound = false;
        }
    }
    
    if(notFound) {
        ImGui::Separator();
        ImGui::Text("No results found!");
    }
}

// Load C style array fonts from memory
// https://www.youtube.com/watch?v=_LXZvuy5olY
ImFont* GUI::staticFontLoader(std::uint8_t* fontData, const std::uint32_t fontLength, float size) {
    ImGuiIO& io = ImGui::GetIO();
    
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    return io.Fonts->AddFontFromMemoryTTF(fontData, fontLength, size, &cfg);
}