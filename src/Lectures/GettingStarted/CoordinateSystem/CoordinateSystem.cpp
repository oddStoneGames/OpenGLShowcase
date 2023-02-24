#include "CoordinateSystem.h"
#include "../../../../vendor/glm/gtc/matrix_transform.hpp"
#include <string>
#include <iostream>
#include <stb_image.h>

namespace GettingStarted
{
    CoordinateSystem::CoordinateSystem(int width, int height) : 
        m_Shader(PROJECT_DIR"/src/Lectures/GettingStarted/CoordinateSystem/CoordinateSystem.vert",
                 PROJECT_DIR"/src/Lectures/GettingStarted/CoordinateSystem/CoordinateSystem.frag")
    {
        m_LectureLink = "https://www.learnopengl.com/Getting-started/Coordinate-Systems";
        
        // Initialize default values.
        Reset(width, height);
    }

    void CoordinateSystem::OpenLecture()
    {
        // Generate VAO, VBO & setup vertex attribute pointers to pass the data to GPU correctly.
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        
        // Pass Buffer Data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_CubeVertices), m_CubeVertices, GL_STATIC_DRAW);

        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex Texture Coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 

        // Generate Texture
        glGenTextures(1, &m_TextureID);        
        // Reset Options.
        Reset(m_Width, m_Height);

        // Set Uniforms.
        m_Shader.Use();
        m_Shader.SetInt("u_Texture", 0);
        m_Shader.SetMat4("u_Model", m_Model);
        m_Shader.SetMat4("u_View", m_View);
        m_Shader.SetMat4("u_Projection", m_Projection);
        Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void CoordinateSystem::RenderLecture(bool settingsVisible, int width, int height)
    {
        if(settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Coordinate System");

            DrawTextCentered("Model Matrix");
            ImGui::DragFloat3(LabelPrefix("Position: ").c_str(), &m_ModelPosition[0], 0.5f, -1000.0f, 1000.0f, "%.1f");
            ImGui::DragFloat3(LabelPrefix("Rotation: ").c_str(), &m_ModelRotation[0], 0.5f, -360.0f, 360.0f, "%.1f");
            ImGui::DragFloat3(LabelPrefix("Scale: ").c_str(), &m_ModelScale[0], 0.01f, -5.0f, 5.0f, "%.2f");
            ImGui::NewLine();

            DrawTextCentered("View Matrix");
            ImGui::DragFloat3(LabelPrefix("Camera Position: ").c_str(), &m_ViewPosition[0], 0.5f, -1000.0f, 1000.0f, "%.1f");
            ImGui::DragFloat3(LabelPrefix("Camera Rotation: ").c_str(), &m_ViewRotation[0], 0.5f, -360.0f, 360.0f, "%.1f");
            ImGui::DragFloat3(LabelPrefix("Camera Scale: ").c_str(), &m_ViewScale[0], 0.01f, -5.0f, 5.0f, "%.2f");
            ImGui::NewLine();

            DrawTextCentered("Projection Matrix");
            // Texture Filtering Mag
            if (ImGui::BeginCombo(LabelPrefix("Type: ").c_str(), m_CurrentProjectionType))
            {
                for (int n = 0; n < IM_ARRAYSIZE(p_ProjectionTypes); n++)
                {
                    bool is_selected = (m_CurrentProjectionType == p_ProjectionTypes[n]);
                    if (ImGui::Selectable(p_ProjectionTypes[n], is_selected))
                        m_CurrentProjectionType = p_ProjectionTypes[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            
            // Show projection options.
            if(strcmp(m_CurrentProjectionType, "Perspective") == 0)
                ImGui::SliderFloat(LabelPrefix("Field of View: ").c_str(), &m_Fov, 0.0f, 89.0f, "%.1f");
            ImGui::DragFloat(LabelPrefix("Width: ").c_str(), &m_Width, 0.5f, -1000.0f, 1000.0f, "%.1f");
            ImGui::DragFloat(LabelPrefix("Height: ").c_str(), &m_Height, 0.5f, -1000.0f, 1000.0f, "%.1f");
            ImGui::DragFloat(LabelPrefix("Near: ").c_str(), &m_Near, 0.5f, -1000.0f, 1000.0f, "%.1f");
            ImGui::DragFloat(LabelPrefix("Far: ").c_str(), &m_Far, 0.5f, -1000.0f, 1000.0f, "%.1f");

            ImGui::NewLine();

            if(DrawButtonCentered("Reset")) Reset(width, height);
            
            ImGui::End();
        }

        // Bind Texture.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        // Reset Matrices.
        m_Model = glm::mat4(1.0f);
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        
        float aspect = m_Width/m_Height;
        m_Projection = strcmp(m_CurrentProjectionType, "Orthographic") == 0 ? 
                        glm::ortho(-aspect, aspect, -1.0f, 1.0f, m_Near, m_Far) :
                        glm::perspective(glm::radians(m_Fov), aspect, m_Near, m_Far);

        m_View = glm::translate(m_View, m_ViewPosition);
        m_View = glm::rotate(m_View, glm::radians(m_ViewRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_View = glm::rotate(m_View, glm::radians(m_ViewRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_View = glm::rotate(m_View, glm::radians(m_ViewRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_View = glm::scale(m_View, m_ViewScale);

        m_Model = glm::translate(m_Model, m_ModelPosition);
        m_Model = glm::rotate(m_Model, glm::radians(m_ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Model = glm::rotate(m_Model, glm::radians(m_ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_Model = glm::rotate(m_Model, glm::radians(m_ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_Model = glm::scale(m_Model, m_ModelScale);

        // Bind Shader.
        m_Shader.Use();

        // Update Shader Uniforms.
        m_Shader.SetMat4("u_Model", m_Model);
        m_Shader.SetMat4("u_View", m_View);
        m_Shader.SetMat4("u_Projection", m_Projection);

        // Bind VAO.
        glBindVertexArray(m_VAO);
        // Draw Indexed.
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        Shader::Unbind();
    }

    void CoordinateSystem::CloseLecture()
    {
        glDeleteTextures(1, &m_TextureID);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        // Reset Depth Testing to false.
        glDisable(GL_DEPTH_TEST);
    }

    void CoordinateSystem::LoadTexture(const char * texturePath)
    {
        // Generate Texture.
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        
        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum colorChannels = GL_RED;
            switch (nrChannels)
            {
                case 3: colorChannels = GL_RGB; break;
                case 4: colorChannels = GL_RGBA; break;
                default: break;
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorChannels, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            if(stbi_failure_reason())
                std::cout << stbi_failure_reason();
            std::cout << "\nFailed to load texture" << std::endl;
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void CoordinateSystem::Reset(int width, int height)
    {
        // Default Texture
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/moai.png");
        m_ModelPosition = glm::vec3(0.0f);
        m_ModelRotation = glm::vec3(10.0f, 20.0f, 0.0f);
        m_ModelScale = glm::vec3(1.0f);
        
        m_ViewPosition = glm::vec3(0.0f, 0.0f, -5.0f);
        m_ViewRotation = glm::vec3(0.0f);
        m_ViewScale = glm::vec3(1.0f);

        m_CurrentProjectionType = "Perspective";

        m_Fov = 45.0f;
        m_Width = width;
        m_Height = height;
        m_Near = 0.1f;
        m_Far = 100.0f;

        m_Model = glm::mat4(1.0f);
        m_Model = glm::rotate(m_Model, glm::radians(m_ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Model = glm::rotate(m_Model, glm::radians(m_ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

        m_View = glm::mat4(1.0f);
        m_View = glm::translate(m_View, m_ViewPosition);

        m_Projection = glm::mat4(1.0f);
        m_Projection = glm::perspective(glm::radians(m_Fov), m_Width/m_Height, m_Near, m_Far);
    }
}