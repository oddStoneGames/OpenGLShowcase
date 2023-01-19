// Hello Window Lecture!
#include "HelloWindow.h"
#include "../../vendor/glad/include/glad.h"
#include "../../vendor/glfw/include/glfw3.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include <iostream>

/// @brief Intialize Lecture & add it to lecture map.
HelloWindow::HelloWindow()
{
    m_LectureLink = "xdg-open https://www.learnopengl.com/Getting-started/Hello-Window";
}

void HelloWindow::OpenLecture()
{
    
}

void HelloWindow::OpenLectureLink()
{
    system(m_LectureLink);
}

void HelloWindow::RenderLecture(bool settingsVisible)
{
    if(settingsVisible)
    {
        // Render ImGui Settings Window!
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
        ImGui::End();
    }

    glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
}