#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "Gui.hpp"
#include "Tracker.hpp"

#include <format>           // std::format

void GUI::displayAllKillerStats(const Tracker& t) const noexcept {
    for(const auto& [killer, data] : t.getMap()) {
        ImGui::Separator();
        ImGui::Text(std::format("Killer: {}\nWins: {}\nPB: {}", killer.c_str(), data.wins, data.personalBest).c_str());
    }
}

void GUI::displayKillerWinstreaksInReferenceToN(const Tracker& t, const int n) const noexcept {
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

void GUI::displayKillerPersonalBestsInReferenceToN(const Tracker& t, const int n) const noexcept {
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