#include "BasicLighting.h"
#include "../../GettingStarted/Camera/Camera.h"
#include "../../../../vendor/glm/gtc/matrix_transform.hpp"
#include <string>
#include <iostream>
#include <stb_image.h>

namespace Lighting
{
    BasicLighting::BasicLighting(int width, int height) : 
        m_LightShader(PROJECT_DIR"/src/Lectures/Lighting/BasicLighting/LightCube.vert",
                      PROJECT_DIR"/src/Lectures/Lighting/BasicLighting/LightCube.frag"), 
        m_CubeShader(PROJECT_DIR"/src/Lectures/Lighting/BasicLighting/Cube.vert",
                     PROJECT_DIR"/src/Lectures/Lighting/BasicLighting/Cube.frag"),
        m_AmbientStrength(0.1f), m_SpecularStrength(0.5f), m_Shininess(32.0f),
        m_LightColor(1.0f), m_CubeColor(glm::vec3(0.2f, 0.4f, 0.6f)), m_LightPosition(0.0f)
    {
        m_LectureLink = "https://www.learnopengl.com/Lighting/Basic-Lighting";
        
        // Initialize default values.
        Reset(width, height);
    }

    void BasicLighting::OpenLecture()
    {
        // Generate VAO, VBO & setup vertex attribute pointers to pass the data to GPU correctly.
        glGenVertexArrays(1, &m_VAO);
        glGenVertexArrays(1, &m_CubeVAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_CubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        
        // Pass Buffer Data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_CubeVertices), m_CubeVertices, GL_STATIC_DRAW);

        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(m_VAO);
        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
      
        // Reset Options.
        Reset(m_Width, m_Height);

        // Set Uniforms.
        m_LightShader.Use();
        m_LightShader.SetMat4("u_Model", m_Model);
        m_LightShader.SetMat4("u_View", m_View);
        m_LightShader.SetMat4("u_Projection", m_Projection);
        GettingStarted::Shader::Unbind();

        m_CubeShader.Use();
        m_CubeShader.SetMat4("u_Model", m_CubeModel);
        m_CubeShader.SetMat4("u_View", m_View); 
        m_CubeShader.SetMat4("u_Projection", m_Projection);
        m_CubeShader.SetFloat3("u_LightColor", m_LightColor);
        m_CubeShader.SetFloat3("u_ObjectColor", m_CubeColor);
        m_CubeShader.SetFloat3("u_LightPos", m_LightPosition);
        m_CubeShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);
        m_CubeShader.SetFloat("u_AmbientStrength", m_AmbientStrength);
        m_CubeShader.SetFloat("u_SpecularStrength", m_SpecularStrength);
        m_CubeShader.SetFloat("u_Shininess", m_Shininess);

        GettingStarted::Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void BasicLighting::RenderLecture(bool settingsVisible, int width, int height)
    {
        GettingStarted::Camera* cam = GettingStarted::Camera::GetInstance();

        if(settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Light Settings");
            ImGui::DragFloat3(LabelPrefix("Light Position: ").c_str(), &m_LightPosition[0], 0.5f, -1000.0f, 1000.0f, "%.2f");
            ImGui::ColorEdit3(LabelPrefix("Light Color: ").c_str(), &m_LightColor[0]);
            ImGui::ColorEdit3(LabelPrefix("Cube Color: ").c_str(), &m_CubeColor[0]);
            ImGui::DragFloat(LabelPrefix("Ambient Strength: ").c_str(), &m_AmbientStrength, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat(LabelPrefix("Specular Strength: ").c_str(), &m_SpecularStrength, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat(LabelPrefix("Shininess: ").c_str(), &m_Shininess, 0.05f, 0.0f, 1000.0f, "%.01f");
            ImGui::NewLine();

            if(DrawButtonCentered("Reset")) Reset(width, height);
            
            ImGui::End();
        }

        // Reset Matrices.
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;
        float aspect = (float)m_Width/(float)m_Height;

        m_Projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        m_View = cam->GetViewMatrix();
        
        // Bind Shader.
        m_LightShader.Use();

        // Update Shader Uniforms.
        m_Model = glm::mat4(1.0f);
        m_Model = glm::translate(m_Model, m_LightPosition);
        m_Model = glm::scale(m_Model, glm::vec3(0.5f));
        m_LightShader.SetMat4("u_Model", m_Model);
        m_LightShader.SetMat4("u_View", m_View);
        m_LightShader.SetMat4("u_Projection", m_Projection);

        // Bind Light VAO.
        glBindVertexArray(m_VAO);
        // Draw Indexed.
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Bind Cube VAO.
        glBindVertexArray(m_CubeVAO);

        m_CubeShader.Use();
        m_CubeShader.SetMat4("u_Model", m_CubeModel);
        m_CubeShader.SetMat4("u_View", m_View);
        m_CubeShader.SetMat4("u_Projection", m_Projection);
        m_CubeShader.SetFloat3("u_LightColor", m_LightColor);
        m_CubeShader.SetFloat3("u_ObjectColor", m_CubeColor);
        m_CubeShader.SetFloat3("u_LightPos", m_LightPosition);
        m_CubeShader.SetFloat3("u_ViewPos", cam->Position);
        m_CubeShader.SetFloat("u_AmbientStrength", m_AmbientStrength);
        m_CubeShader.SetFloat("u_SpecularStrength", m_SpecularStrength);
        m_CubeShader.SetFloat("u_Shininess", m_Shininess);

        // Draw Indexed.
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        GettingStarted::Shader::Unbind();
    }

    void BasicLighting::CloseLecture()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteVertexArrays(1, &m_CubeVAO);
        glDeleteBuffers(1, &m_VBO);

        // Reset Depth Testing to false.
        glDisable(GL_DEPTH_TEST);
    }

    void BasicLighting::Reset(int width, int height)
    {
        // Default Matrices
        m_Model = glm::mat4(1.0f);
        m_CubeModel = glm::mat4(1.0f);
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;

        // Reset Options
        m_AmbientStrength = 0.1f;
        m_SpecularStrength = 0.5f;
        m_Shininess = 32.0f;
        m_LightColor = glm::vec3(1.0f);
        m_CubeColor = glm::vec3(0.2f, 0.4f, 0.6f);
        m_LightPosition = glm::vec3(0.0f);

        m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, 3.0f));
        m_Model = glm::scale(m_Model, glm::vec3(0.5f));
        m_CubeModel = glm::translate(m_CubeModel, glm::vec3(0.7f, -0.3f, -1.1f));
        m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/(float)m_Height, 0.1f, 100.0f);
    }
}