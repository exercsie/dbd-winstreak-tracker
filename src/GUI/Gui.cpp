#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "Gui.hpp"
#include "Tracker.hpp"

#include <cstdint>          // std::uint16_t
#include <expected>         // std::expected, std::unexpected
#include <format>           // std::format
#include <GLFW/glfw3.h>     // gl apis
#include <optional>         // std::optional
#include <print>            // std::println
#include <string>           // std::string

std::expected<void, std::string> GUI::inputHandling(const int firstParam, const int secondParam, const std::string& errorMsg) {
    if(firstParam == secondParam) {
        return std::unexpected(std::format("{}", errorMsg));
    }

    return {};
}

int main() {
    GUI g;
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "dbd-winstreak-tracker", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    ImGui::CreateContext();

    ImGui::GetIO().FontGlobalScale = 1.5f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Tracker t;
    std::string error;
    std::string selectedKiller;
    bool killerSelected = false;
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(500.0f, displaySize.y), ImGuiCond_Always);
        
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Welcome to dbd winstreak tracker!");
        ImGui::Text("Enter your killer: ");
        
        static char buffer[128]{};
        if(ImGui::InputText("##", buffer, sizeof(buffer))) {
            killerSelected = false;
        }
        
        static std::optional<GUI::UI> button;

        if(ImGui::Button("Select killer")) {
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

        if(!killerSelected) {
            button.reset();
        }

        if(!error.empty()) {
            ImGui::TextColored(ImVec4(1, .3f, .3f, 1), "%s", error.c_str());
        }

        ImGui::Text("Selected killer: %s", killerSelected ? selectedKiller.c_str() : "none");
        ImGui::Separator();

        if(killerSelected) {
            if(ImGui::Button("Winstreak Counter", ImVec2(-1, 0))) {
                button = GUI::UI::counter;
            }
    
            if(ImGui::Button(std::format("View {}'s stats", selectedKiller).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::viewStats;
            }
    
            if(ImGui::Button(std::format("Reset {}'s stats", selectedKiller).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::resetStats;
            }
    
            if(ImGui::Button(std::format("Set {}'s stats", selectedKiller).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::setStats;
            }
    
            if(ImGui::Button("Query stats", ImVec2(-1, 0))) {
                button = GUI::UI::query;
            }
        }
        
        if(button) {
            switch(*button) {
                case GUI::UI::counter: {
                    ImGui::Text("Wins: %d PB: %d", t.getWins(), t.getPersonalBest());
                    if(ImGui::Button("+1")) {
                        t.incrementWins();
                        if(!error.empty()) {
                            error.clear();
                        }
                    }

                    ImGui::SameLine();
                    if(ImGui::Button("-1")) {
                        const std::expected r = t.decrementWins();
                        if(!r) {
                            error = r.error();
                        }
                    }
    
                    break;
                }
    
                case GUI::UI::viewStats: {
                    if(!error.empty()) {
                        error.clear();
                    }
                    ImGui::Text(std::format("Killer: {}\nWins: {}\nPB: {}", selectedKiller, t.getWins(), t.getPersonalBest()).c_str());
                    break;
                }
            }
        }

        ImGui::End();
        
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}