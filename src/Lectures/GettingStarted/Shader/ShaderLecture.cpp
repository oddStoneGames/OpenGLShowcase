#include "ShaderLecture.h"
#include <math.h>

namespace GettingStarted
{
    ShaderLecture::ShaderLecture() : 
        //using '../' because the current working directory is bin.
        m_Shader("../src/Lectures/GettingStarted/Shader/Triangle.vert", 
                 "../src/Lectures/GettingStarted/Shader/Triangle.frag")
    {
        m_LectureLink = "xdg-open https://www.learnopengl.com/Getting-started/Shaders";
    }

    void ShaderLecture::OpenLecture()
    {
        // Create VAO & VBOs.
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        // Pass Triangle Data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_TriangleVertices), m_TriangleVertices, GL_STATIC_DRAW);

        // Set Vertex Attributes to Pass the Data to the GPU accordingly.
        // Vertex Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);

        m_Shader.Use();
        //Set Default values for uniforms.
        m_Shader.SetFloat2("u_Offset", 0.0f, 0.0f);
        m_Shader.SetInt("u_Invert", 1);
        m_Shader.SetFloat3("u_Color", 1.0f, 1.0f, 1.0f);
        Shader::Unbind();
    }

    void ShaderLecture::RenderLecture(bool settingsVisible, int width, int height)
    {
        if(settingsVisible)
        {
            // Draw ImGui Layer.
            ImGui::Begin("Shaders");
            ImGui::Checkbox(LabelPrefix("Animate: ").c_str(), &m_Animate);
            ImGui::Checkbox(LabelPrefix("Invert: ").c_str(), &m_Invert);
            ImGui::DragFloat2(LabelPrefix("Offset: ").c_str(), m_Offset, 0.01f, -1.0f, 1.0f, "%.2f");
            ImGui::End();
        }

        // Bind Shader & VAO
        m_Shader.Use();
        glBindVertexArray(m_VAO);

        // Update Uniforms.
        if(m_Animate)
        {

            float timeValue = glfwGetTime();
            float redValue = cos(timeValue + 0.1f) / 2.0f + 0.5f;
            float greenValue = sin(timeValue + 0.1f) / 2.0f + 0.5f;
            float blueValue = cos(timeValue + 0.1f) - sin(timeValue + 0.1f);
            m_Shader.SetFloat3("u_Color", redValue, greenValue, blueValue);
        }else
        {
            // Reset Color.
            m_Shader.SetFloat3("u_Color", 1.0f, 1.0f, 1.0f);
        }

        m_Shader.SetFloat2("u_Offset", m_Offset[0], m_Offset[1]);
        m_Shader.SetInt("u_Invert", m_Invert ? -1 : 1);

        // Render Triangle.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        Shader::Unbind();
    }

    /// @brief Cleanup.
    void ShaderLecture::CloseLecture()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }
}