// This is the base file of the program.
// It is responsible for the ImGui layout & initializing GLFWwindow & openGL context.

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

#include "Lectures/GettingStarted/HelloWindow/HelloWindow.h"
#include "Lectures/GettingStarted/HelloTriangle/HelloTriangle.h"
#include "Lectures/GettingStarted/Shader/ShaderLecture.h"
#include "Lectures/GettingStarted/Texture/TextureLecture.h"
#include "Lectures/GettingStarted/Transformations/Transformations.h"
#include "Lectures/GettingStarted/CoordinateSystem/CoordinateSystem.h"
#include "Lectures/GettingStarted/Camera/CameraLecture.h"

#include "Lectures/Lighting/Colors/ColorLecture.h"
#include "Lectures/Lighting/BasicLighting/BasicLighting.h"
#include "Lectures/Lighting/Materials/Materials.h"
#include "Lectures/Lighting/LightingMaps/LightingMaps.h"
#include "Lectures/Lighting/LightCasters/LightCasters.h"
#include "Lectures/Lighting/MultipleLights/MultipleLights.h"
#include "Lectures/ModelLoading/ModelLoadingLecture.h"

#include "Lectures/AdvancedOpenGL/DepthTesting/DepthTesting.h"
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
    float m_DeltaTime = 0.0f, m_LastTime = 0.0f;
    const uint32_t WIDTH = 1080; 
    const uint32_t HEIGHT = 608;
    const uint32_t REFRESHRATE = 60;
    uint32_t m_CurrentLectureIndex = 0;
    bool m_FirstMouse = true;
    float m_LastX = WIDTH / 2.0f, m_LastY = HEIGHT / 2.0f;
private:
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void ResizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput();

    /// @brief Used to get callbacks from GLFW which expects static functions.
    class GLFWCallbackWrapper
        {
        public:
            GLFWCallbackWrapper() = delete;
            ~GLFWCallbackWrapper() = delete;

            static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
            static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
            static void ResizeCallback(GLFWwindow* window, int width, int height);
            static void SetApplication(ShowcaseApplication *application);
        private:
            static ShowcaseApplication* s_application;
        };
};