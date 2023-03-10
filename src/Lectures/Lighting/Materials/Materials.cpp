#include "Materials.h"
#include "../../GettingStarted/Camera/Camera.h"
#include "../../../../vendor/glm/gtc/matrix_transform.hpp"

namespace Lighting
{
    Materials::Materials(int width, int height) : 
        m_LightShader(PROJECT_DIR"/src/Lectures/Lighting/Materials/LightCube.vert",
                      PROJECT_DIR"/src/Lectures/Lighting/Materials/LightCube.frag"), 
        m_CubeShader(PROJECT_DIR"/src/Lectures/Lighting/Materials/Materials.vert",
                     PROJECT_DIR"/src/Lectures/Lighting/Materials/Materials.frag"),
        m_LightAmbientColor(0.1f), m_LightDiffuseColor(0.5f), m_LightSpecularColor(1.0f),
        m_MaterialAmbientColor(glm::vec3(0.2f, 0.4f, 0.6f)), m_MaterialDiffuseColor(glm::vec3(0.2f, 0.4f, 0.6f)), 
        m_MaterialSpecularColor(0.5f), m_MaterialShininess(32.0f)
    {
        m_LectureLink = "https://www.learnopengl.com/Lighting/Materials";

        // Initialize default values.
        Reset(width, height);
    }

    void Materials::OpenLecture()
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
        m_LightShader.SetFloat3("u_LightColor", m_LightDiffuseColor);
        GettingStarted::Shader::Unbind();

        m_CubeShader.Use();
        m_CubeShader.SetMat4("u_Model", m_CubeModel);
        m_CubeShader.SetMat4("u_View", m_View); 
        m_CubeShader.SetMat4("u_Projection", m_Projection);
        m_CubeShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_CubeShader.SetFloat3("u_Light.position", m_LightPosition);
        m_CubeShader.SetFloat3("u_Light.ambient", m_LightAmbientColor);
        m_CubeShader.SetFloat3("u_Light.diffuse", m_LightDiffuseColor);
        m_CubeShader.SetFloat3("u_Light.specular", m_LightSpecularColor);
        m_CubeShader.SetFloat3("u_Material.ambient", m_MaterialAmbientColor);
        m_CubeShader.SetFloat3("u_Material.diffuse", m_MaterialDiffuseColor);
        m_CubeShader.SetFloat3("u_Material.specular", m_MaterialSpecularColor);
        m_CubeShader.SetFloat("u_Material.shininess", m_MaterialShininess);

        GettingStarted::Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void Materials::RenderLecture(bool settingsVisible, int width, int height)
    {
        GettingStarted::Camera* cam = GettingStarted::Camera::GetInstance();

        if(settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Material Lecture");

            DrawTextCentered("Light Settings");
            ImGui::NewLine();
            ImGui::DragFloat3(LabelPrefix("Position: ").c_str(), &m_LightPosition[0], 0.5f, -1000.0f, 1000.0f, "%.2f");
            ImGui::ColorEdit3(LabelPrefix("Ambient Color: ").c_str(), &m_LightAmbientColor[0]);
            ImGui::ColorEdit3(LabelPrefix("Diffuse Color: ").c_str(), &m_LightDiffuseColor[0]);
            ImGui::ColorEdit3(LabelPrefix("Specular Color: ").c_str(), &m_LightSpecularColor[0]);
            ImGui::NewLine();

            DrawTextCentered("Material Settings");
            ImGui::NewLine();
            ImGui::ColorEdit3(LabelPrefix("Ambient: ").c_str(), &m_MaterialAmbientColor[0]);
            ImGui::ColorEdit3(LabelPrefix("Diffuse: ").c_str(), &m_MaterialDiffuseColor[0]);
            ImGui::ColorEdit3(LabelPrefix("Specular: ").c_str(), &m_MaterialSpecularColor[0]);
            ImGui::DragFloat(LabelPrefix("Shininess: ").c_str(), &m_MaterialShininess, 0.05f, 0.0f, 1000.0f, "%.2f");
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
        m_LightShader.SetFloat3("u_LightColor", m_LightDiffuseColor);

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
        m_CubeShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_CubeShader.SetFloat3("u_Light.position", m_LightPosition);
        m_CubeShader.SetFloat3("u_Light.ambient", m_LightAmbientColor);
        m_CubeShader.SetFloat3("u_Light.diffuse", m_LightDiffuseColor);
        m_CubeShader.SetFloat3("u_Light.specular", m_LightSpecularColor);
        m_CubeShader.SetFloat3("u_Material.ambient", m_MaterialAmbientColor);
        m_CubeShader.SetFloat3("u_Material.diffuse", m_MaterialDiffuseColor);
        m_CubeShader.SetFloat3("u_Material.specular", m_MaterialSpecularColor);
        m_CubeShader.SetFloat("u_Material.shininess", m_MaterialShininess);

        // Draw Indexed.
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        GettingStarted::Shader::Unbind();
    }

    void Materials::CloseLecture()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteVertexArrays(1, &m_CubeVAO);
        glDeleteBuffers(1, &m_VBO);

        // Reset Depth Testing to false.
        glDisable(GL_DEPTH_TEST);
    }

    void Materials::Reset(int width, int height)
    {
        // Default Matrices
        m_Model = glm::mat4(1.0f);
        m_CubeModel = glm::mat4(1.0f);
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;

        // Reset Options
        m_LightPosition = glm::vec3(0.0f);
        m_LightAmbientColor = glm::vec3(0.1f);
        m_LightDiffuseColor = glm::vec3(0.5f);
        m_LightSpecularColor = glm::vec3(1.0f);
        m_MaterialAmbientColor = glm::vec3(0.2f, 0.4f, 0.6f);
        m_MaterialDiffuseColor = glm::vec3(0.2f, 0.4f, 0.6f);
        m_MaterialSpecularColor = glm::vec3(0.5f);
        m_MaterialShininess = 32.0f;

        m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, 3.0f));
        m_Model = glm::scale(m_Model, glm::vec3(0.5f));
        m_CubeModel = glm::translate(m_CubeModel, glm::vec3(0.7f, -0.3f, -1.1f));
        m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/(float)m_Height, 0.1f, 100.0f);
    }
}