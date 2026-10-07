#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "Gui.hpp"

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
        ImGui::InputText("##", buffer, sizeof(buffer));
        
        const std::string killerName = buffer;
        const bool hasKiller = !killerName.empty();

        ImGui::Text("Selected killer: %s", hasKiller ? killerName.c_str() : "none");
        ImGui::Separator();

        static std::optional<GUI::UI> button;
        if(!hasKiller) {
            button.reset();
        }

        if(hasKiller) {
            if(ImGui::Button("Winstreak Counter", ImVec2(-1, 0))) {
                button = GUI::UI::counter;
            }
    
            if(ImGui::Button(std::format("View {}'s stats", killerName).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::viewStats;
            }
    
            if(ImGui::Button(std::format("Reset {}'s stats", killerName).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::resetStats;
            }
    
            if(ImGui::Button(std::format("Set {}'s stats", killerName).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::setStats;
            }
    
            if(ImGui::Button("Query stats", ImVec2(-1, 0))) {
                button = GUI::UI::query;
            }
        }
        
        if(button) {
            switch(*button) {
                case GUI::UI::counter: {
                    static std::uint16_t counter{};
                    if(ImGui::Button("+1")) {
                        ++counter;
                        std::println("Counter: {}", counter);
                    }
    
                    if(ImGui::Button("-1")) {
                        const auto result = g.inputHandling(counter, 0, "Cannot go past zero!");
                        if(!result) {
                            std::println("{}", result.error());
                        } else {
                            --counter;
                            std::println("Counter: {}", counter);
                        }
                    }
    
                    break;
                }
    
                case GUI::UI::viewStats: {
                    ImGui::Text(std::format("Killer: {}\nWins: 0\nPB: 2", killerName).c_str());
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