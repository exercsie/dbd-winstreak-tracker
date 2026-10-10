#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "Gui.hpp"
#include "Tracker.hpp"

#include <cstdint>          // std::uint32_t
#include <expected>         // std::expected
#include <format>           // std::format
#include <optional>         // std::optional
#include <string>           // std::string

void GUI::MenuView(Tracker& t, ImVec2& displaySize, bool& killerSelected, std::string& selectedKiller, std::string& error, std::string& success) {
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(370.0f, displaySize.y), ImGuiCond_Always);
    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Welcome to dbd winstreak tracker!");
    ImGui::Text("Enter your killer: ");

    static char buffer[128]{};
    const bool enter = ImGui::InputText("##", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);

    // Update UI in real-time as a killer is entered
    if(ImGui::IsItemEdited()) {
        killerSelected = false;
    }

    if(ImGui::SameLine(); ImGui::Button("Select killer") || enter) {
        t.setKiller(t.killerNormalisation(buffer));
        t.buildKillerWinMap();
        killerSelected = t.isValidKiller();
        if(killerSelected) {
            selectedKiller = t.killerNormalisation(buffer);
            // Remove prior error if killer name was entered incorrectly
            error.clear();
        } else {
            error = std::format("{} does not exist. Example: \"The Terrifier\".", buffer);
        }
    }
    
    static std::optional<GUI::UI> button;
    if(!killerSelected) {
        button.reset();
    }

    if(!success.empty()) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1), "%s", success.c_str());
    }

    if(!error.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1), "%s", error.c_str());
    }

    if(killerSelected) {
        ImGui::Text(std::format("Selected killer: {}", killerSelected ? selectedKiller : "none").c_str());
        ImGui::Separator();
        if(ImGui::Button("Winstreak Counter", ImVec2(-1, 0))) {
            button = GUI::UI::counter;
            success.clear();
            error.clear();
        }

        if(ImGui::Button(std::format("Reset {}'s stats", selectedKiller).c_str(), ImVec2(-1, 0))) {
            button = GUI::UI::resetStats;
            success.clear();
            error.clear();
        }

        if(ImGui::Button(std::format("Set {}'s stats", selectedKiller).c_str(), ImVec2(-1, 0))) {
            button = GUI::UI::setStats;
            success.clear();
            error.clear();
        }

        if(ImGui::Button("Query stats", ImVec2(-1, 0))) {
            button = GUI::UI::query;
            success.clear();
            error.clear();
        }
    }
    
    if(button) {
        switch(*button) {
            case GUI::UI::counter: {
                CounterOption(t, error, selectedKiller);
                break;
            }

            case GUI::UI::resetStats: {
                ResetStatsOption(t, error, success, selectedKiller);
                break;
            }

            case GUI::UI::setStats: {
                SetStatsOption(t, error, success);
                break;
            }

            case GUI::UI::query: {
                QueryOption(t, displaySize, selectedKiller);
                break;
            }
        }
    }

    ImGui::End();
}

void GUI::KillerView(ImVec2& displaySize) {
    ImGui::SetNextWindowPos(ImVec2(370.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y), ImGuiCond_Always);
    ImGui::Begin("Killer view", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::End();
}

bool GUI::buttonColour(const char* name, ImVec4 v, ImVec2 size) {
    ImGui::PushStyleColor(ImGuiCol_Button, v);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(v.x + 0.1f, v.y + 0.1f, v.z + 0.1f, 1.0f)); // add +.1 on hover
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(v.x - 0.1f, v.y - 0.1f, v.z - 0.1f, 1.0f)); // decrement -.1 on press to simulate diff states
    const bool isButtonPressed = ImGui::Button(name, size);
    ImGui::PopStyleColor(3);
    return isButtonPressed;
}

void GUI::CounterOption(Tracker& t, std::string& error, const std::string& selectedKiller) {
    ImGui::Text(std::format("{}'s stats: ", selectedKiller).c_str());
    ImGui::Separator();
    ImGui::Text(std::format("Wins: {}\nPB: {}", t.getWins(), t.getPersonalBest()).c_str());
    if(buttonColour("+1", ImVec4(0.0f, 1.0f, 0.0f, 0.1f))) {
        t.incrementWins();
        if(!error.empty()) {
            error.clear();
        }
    }

    ImGui::SameLine();
    if(buttonColour("-1", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
        const std::expected r = t.decrementWins();
        if(!r) {
            error = r.error();
        }
    }
}

void GUI::ResetStatsOption(Tracker& t, std::string& error, std::string& success, const std::string& selectedKiller) {
    if(ImGui::Button("Reset winstreak")) {
        ImGui::OpenPopup("Confirm winstreak reset");
    }

    ImGui::SameLine();
    if(ImGui::Button("Reset personal best")) {
        ImGui::OpenPopup("Confirm PB reset");
    }

    if(ImGui::BeginPopupModal("Confirm winstreak reset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(std::format("Reset {}'s winstreak?", selectedKiller).c_str());
        if(buttonColour("Yes", ImVec4(0.0f, 1.0f, 0.0f, 0.1f))) {
            const std::expected r = t.resetWinstreak();
            if(r) {
                success = r.value();
                error.clear();
            } else {
                error = r.error();
                success.clear();
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if(buttonColour("No", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
            error.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if(ImGui::BeginPopupModal("Confirm PB reset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(std::format("Reset {}'s personal best? (This resets current wins)", selectedKiller).c_str());
        if(buttonColour("Yes", ImVec4(0.0f, 1.0f, 0.0f, 0.1f))) {
            const std::expected r = t.resetPersonalBest();
            if(r) {
                success = r.value();
                error.clear();
            } else {
                error = r.error();
                success.clear();
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if(buttonColour("No", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
            error.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void GUI::SetStatsOption(Tracker& t, std::string& error, std::string& success) {
    if(ImGui::Button("Set winstreak")) {
        ImGui::OpenPopup("Enter winstreak value");
    }
    
    static int tempWins{};
    if(ImGui::BeginPopupModal("Enter winstreak value", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputInt("##", &tempWins, 1, 10);
        const bool enter = ImGui::IsKeyPressed(ImGuiKey_Enter);
        if(buttonColour("Apply", ImVec4(0.0f, 1.0f, 0.0f, 0.1f)) || enter) {
            const std::expected r = t.setWins(tempWins);
            if(r) {
                tempWins = 0;
                success = r.value();
                error.clear();
            } else {
                tempWins = 0;
                error = r.error();
                success.clear();
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(buttonColour("Cancel", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
            tempWins = 0;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::SameLine();

    if(ImGui::Button("Set personal best")) {
        ImGui::OpenPopup("Enter personal best value");
    }

    static int tempPB{};
    if(ImGui::BeginPopupModal("Enter personal best value", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputInt("##", &tempPB, 1, 10);
        const bool enter = ImGui::IsKeyPressed(ImGuiKey_Enter);
        if(buttonColour("Apply", ImVec4(0.0f, 1.0f, 0.0f, 0.1f)) || enter) {
            const std::expected r = t.setPersonalBest(tempPB);
            if(r) {
                tempPB = 0;
                success = r.value();
                error.clear();
            } else {
                tempPB = 0;
                error = r.error();
                success.clear();
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(buttonColour("Cancel", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
            tempPB = 0;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void GUI::QueryOption(const Tracker& t, ImVec2& displaySize, const std::string& selectedKiller) {
    static int n{};

    ImGui::SetNextWindowPos(ImVec2(370.0f, 850.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(displaySize.x - 370.0f, displaySize.y - 850.0f), ImGuiCond_Always); // - by window pos to ensure all vals are on the screen
    ImGui::Begin("Query view", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if(ImGui::BeginTabBar("Query tabs")) {
        if(ImGui::BeginTabItem(std::format("{}'s stats", selectedKiller).c_str())) {
            ImGui::Text(std::format("Killer: {}\nWins: {}\nPB: {}", selectedKiller, t.getWins(), t.getPersonalBest()).c_str());
            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("All killers")) {
            displayAllKillerStats(t);
            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("Wins >= N")) {
            ImGui::InputInt("##", &n);
            if(n < 0) {
                n = 0;
            }

            displayKillerWinstreaksInReferenceToN(t, n);
            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("Personal bests >= N")) {
            ImGui::InputInt("##", &n);
            if(n < 0) {
                n = 0;
            }
            
            displayKillerPersonalBestsInReferenceToN(t, n);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
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