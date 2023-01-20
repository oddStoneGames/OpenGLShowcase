// This is the base file of the program.
// It is responsible for the ImGui layout & initializing GLFWwindow & openGL context.

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "../vendor/glfw/include/glfw3.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

#include "Lectures/HelloWindow.h"
#include "Lectures/HelloTriangle.h"

class ShowcaseApplication
{
public:
    void Run();
private:
    bool Init();
    void InitImGui();
    void SetCustomStyle();
    void RenderLoop();
    void Fullscreen();
    void SwitchCursorLock();
    void ShowHideFPS();
    void ShowHideMenu();
    void ShowHideSettings();
    void Quit();
    void ImGuiBeginFrame();
    void ImGuiRender();
    void ImGuiEndFrame();
    void Cleanup();
private:
    GLFWwindow* m_Window;
    bool m_Fullscreen = false;
    bool m_CursorLocked = false;
    bool m_FPSVisible = true, m_MenuVisible = true, m_SettingsVisible = true;
    const uint32_t WIDTH = 1080; 
    const uint32_t HEIGHT = 608;
    const uint32_t REFRESHRATE = 60;
    uint32_t m_CurrentLectureIndex = 0;
private:
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /// @brief Used to get callbacks from GLFW which expects static functions.
    class GLFWCallbackWrapper
        {
        public:
            GLFWCallbackWrapper() = delete;
            ~GLFWCallbackWrapper() = delete;

            static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void SetApplication(ShowcaseApplication *application);
        private:
            static ShowcaseApplication* s_application;
        };
};