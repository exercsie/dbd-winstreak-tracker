#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "Gui.hpp"          // Includes font headers
#include "Tracker.hpp"

#include <format>           // std::format
#include <GLFW/glfw3.h>     // gl apis
#include <string>           // std::string

int main() {
    GUI g;
    glfwInit();

    // Start maximised 
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(950, 1000, "dbd-winstreak-tracker", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    // Load custom fonts
    ImFont* notoSansFont = g.staticFontLoader(NotoSans_Regular_ttf, NotoSans_Regular_ttf_len, 25.0f);
    //ImFont* newFont = g.staticFontLoader(NewFont_Regular_ttf, NewFont_Regular_ttf_len, x.yf);

    // default ImGui font
    ImFontConfig cfg;
    cfg.SizePixels = 25.0f;
    ImFont* defaultImGuiFont = io.Fonts->AddFontDefault(&cfg);

    // Set default font
    io.FontDefault = notoSansFont;

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

        /*ImGui::SetNextWindowPos(viewPort->WorkPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(viewPort->WorkSize, ImGuiCond_Always);
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);*/

        // Menu view
        g.MenuView(t, displaySize, killerSelected, selectedKiller, error);
        
        // Killer View
        g.KillerView(displaySize);

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