// This is the base file of the program.
// It is responsible for the ImGui layout & initializing GLFWwindow & openGL context.

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "../vendor/glad/include/glad.h"
#include "../vendor/glfw/include/glfw3.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

class ShowcaseApplication
{
public:
    void Run();
private:
    bool Init();
    void InitImGui();
    void SetCustomStyle();
    void RenderLoop();
    void ProcessInput();
    void Fullscreen();
    void SwitchCursorLock();
    void ShowHideFPS();
    void ShowHideMenu();
    void ShowHideSettings();
    void OpenCurrentLecture();
    void Quit();
    void ImGuiBeginFrame();
    void ImGuiRender();
    void ImGuiEndFrame();
    void Cleanup();
private:
    GLFWwindow* m_Window;
    bool m_Fullscreen = false;
    bool m_FPSVisible = true, m_MenuVisible = true, m_SettingsVisible = true;
    bool m_PressedOnce = false;
    uint32_t m_FramesPassedSinceKeyPressed = 0;
    const uint32_t WIDTH = 1080; 
    const uint32_t HEIGHT = 608;
    const uint32_t REFRESHRATE = 60;
};