// Hello Window Lecture!
#include "HelloWindow.h"

namespace GettingStarted
{
    /// @brief Intialize Lecture Link & all the other variables necessary.
    HelloWindow::HelloWindow()
    {
        m_LectureLink = "https://www.learnopengl.com/Getting-started/Hello-Window";
    }

    void HelloWindow::OpenLecture()
    {
        // Reset ClearColor.
        for(int i = 0; i < 4; i++)
            m_ClearColor[i] = i < 3 ? 0.1f : 1.0f;
    }

    void HelloWindow::RenderLecture(bool settingsVisible, int width, int height)
    {
        if(settingsVisible)
        {
            // Render ImGui Settings Window!
            ImGui::Begin("Hello Window");
            ImGui::ColorEdit4("Clear Color", m_ClearColor);
            ImGui::End();
        }

        // Set Clear Color.
        glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    }

    /// @brief Free used resources!
    void HelloWindow::CloseLecture()
    {
        // Reset Clear Color.
        for(int i = 0; i < 4; i++)  m_ClearColor[i] = i < 3 ? 0.1f : 1.0f;
        glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    }
}