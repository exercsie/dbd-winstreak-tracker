#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "Gui.hpp"
#include "Tracker.hpp"

#include <expected>         // std::expected
#include <format>           // std::format
#include <GLFW/glfw3.h>     // gl apis
#include <optional>         // std::optional
#include <string>           // std::string

int main() {
    GUI g;
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1000, 700, "dbd-winstreak-tracker", nullptr, nullptr);

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


        const ImGuiViewport* viewPort = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewPort->WorkPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(viewPort->WorkSize, ImGuiCond_Always);
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        // side bar
        /*ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(550.0f, displaySize.y), ImGuiCond_Always);
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);*/

        ImGui::Text("Welcome to dbd winstreak tracker!");
        ImGui::Text("Enter your killer: ");
        
        
        static char buffer[128]{};
        const bool enter = ImGui::InputText("##", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);

        // Update UI in real-time as a killer is entered
        if(ImGui::IsItemEdited()) {
            killerSelected = false;
        }

        if(ImGui::Button("Select killer") || enter) {
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

        if(!error.empty()) {
            ImGui::TextColored(ImVec4(1, 0.3f, 0.3f, 1), "%s", error.c_str());
        }

        if(killerSelected) {
            ImGui::Text(std::format("Selected killer: {}", killerSelected ? selectedKiller : "none").c_str());
            ImGui::Separator();
            if(ImGui::Button("Winstreak Counter", ImVec2(-1, 0))) {
                button = GUI::UI::counter;
            }
    
            if(ImGui::Button(std::format("Reset {}'s stats", selectedKiller).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::resetStats;
                error.clear();
            }
    
            if(ImGui::Button(std::format("Set {}'s stats", selectedKiller).c_str(), ImVec2(-1, 0))) {
                button = GUI::UI::setStats;
                error.clear();
            }
    
            if(ImGui::Button("Query stats", ImVec2(-1, 0))) {
                button = GUI::UI::query;
                error.clear();
            }
        }
        
        if(button) {
            switch(*button) {
                case GUI::UI::counter: {
                    ImGui::Text(std::format("{}'s stats: ", selectedKiller).c_str());
                    ImGui::Separator();
                    ImGui::Text(std::format("Wins: {}\nPB: {}", t.getWins(), t.getPersonalBest()).c_str());
                    if(g.buttonColour("+1", ImVec4(0.0f, 1.0f, 0.0f, 0.1f))) {
                        t.incrementWins();
                        if(!error.empty()) {
                            error.clear();
                        }
                    }

                    ImGui::SameLine();
                    if(g.buttonColour("-1", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
                        const std::expected r = t.decrementWins();
                        if(!r) {
                            error = r.error();
                        }
                    }
    
                    break;
                }
    
                case GUI::UI::resetStats: {
                    if(ImGui::Button("Reset winstreak")) {
                        ImGui::OpenPopup("Confirm winstreak reset");
                    }

                    ImGui::SameLine();
                    if(ImGui::Button("Reset personal best")) {
                        ImGui::OpenPopup("Confirm PB reset");
                    }

                    if(ImGui::BeginPopupModal("Confirm winstreak reset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                        ImGui::Text(std::format("Reset {}'s winstreak?", selectedKiller).c_str());
                        if(g.buttonColour("Yes", ImVec4(0.0f, 1.0f, 0.0f, 0.1f))) {
                            const std::expected r = t.resetWinstreak();
                            if(!r) {
                                error = r.error();
                            } else {
                                error.clear();
                            }

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();
                        if(g.buttonColour("No", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
                            error.clear();
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }

                    if(ImGui::BeginPopupModal("Confirm PB reset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                        ImGui::Text(std::format("Reset {}'s personal best? (This resets current wins)", selectedKiller).c_str());
                        if(g.buttonColour("Yes", ImVec4(0.0f, 1.0f, 0.0f, 0.1f))) {
                            const std::expected r = t.resetPersonalBest();
                            if(!r) {
                                error = r.error();
                            } else {
                                error.clear();
                            }

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();
                        if(g.buttonColour("No", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
                            error.clear();
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }

                    break;
                }

                case GUI::UI::setStats: {
                    static int tempWins{};
                    if(ImGui::Button("Set winstreak")) {
                        ImGui::OpenPopup("Enter winstreak value");
                    }

                    if(ImGui::BeginPopupModal("Enter winstreak value", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                        const bool enter = ImGui::InputInt("##", &tempWins, ImGuiInputTextFlags_EnterReturnsTrue);
                        if(g.buttonColour("Apply", ImVec4(0.0f, 1.0f, 0.0f, 0.1f)) || enter) {
                            const std::expected r = t.setWins(tempWins);
                            if(!r) {
                                tempWins = 0;
                                error = r.error();
                            } else {
                                tempWins = 0;
                                error.clear();
                            }

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();

                        if(g.buttonColour("Cancel", ImVec4(1.0f, 0.0f, 0.0f, 0.1f))) {
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
                        const bool enter = ImGui::InputInt("##", &tempPB, ImGuiInputTextFlags_EnterReturnsTrue);
                        if(g.buttonColour("Apply", ImVec4(0.0f, 255.0f, 0.0f, 0.1f)) || enter) {
                            const std::expected r = t.setPersonalBest(tempPB);
                            if(!r) {
                                tempPB = 0;
                                error = r.error();
                            } else {
                                tempPB = 0;
                                error.clear();
                            }

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();

                        if(g.buttonColour("Cancel", ImVec4(255.0f, 0.0f, 0.0f, 0.1f))) {
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }

                    break;
                }

                case GUI::UI::query: {
                    static int n{};

                    if(ImGui::BeginTabBar("Query tabs")) {
                        if(ImGui::BeginTabItem(std::format("{}'s stats", selectedKiller).c_str())) {
                            ImGui::Text(std::format("Killer: {}\nWins: {}\nPB: {}", selectedKiller, t.getWins(), t.getPersonalBest()).c_str());
                            ImGui::EndTabItem();
                        }

                        if(ImGui::BeginTabItem("All killers")) {
                            g.displayAllKillerStats(t);
                            ImGui::EndTabItem();
                        }
    
                        if(ImGui::BeginTabItem("Wins >= N")) {
                            ImGui::InputInt("##", &n);
                            if(n < 0) {
                                n = 0;
                            }

                            g.displayKillerWinstreaksInReferenceToN(t, n);
                            ImGui::EndTabItem();
                        }
    
                        if(ImGui::BeginTabItem("Personal bests >= N")) {
                            ImGui::InputInt("##", &n);
                            if(n < 0) {
                                n = 0;
                            }
                            
                            g.displayKillerPersonalBestsInReferenceToN(t, n);
                            ImGui::EndTabItem();
                        }

                        ImGui::EndTabBar();
                    }

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