#include "Transformations.h"
#include "../../../../vendor/glm/gtc/matrix_transform.hpp"
#include <string>
#include <iostream>
#include <stb_image.h>

namespace GettingStarted
{
    Transformations::Transformations() : 
        //using '../' because the current working directory is bin.
        m_Shader("../src/Lectures/GettingStarted/Transformations/Transformations.vert",
                 "../src/Lectures/GettingStarted/Transformations/Transformations.frag")
    {
        m_LectureLink = "xdg-open https://www.learnopengl.com/Getting-started/Transformations";
        
        // Initialize default values.
        Reset();
    }

    void Transformations::OpenLecture()
    {
        // Generate VAO, VBO & EBO & setup vertex attribute pointers to pass the data to GPU correctly.
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        
        // Pass Quad Data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_QuadVertices), m_QuadVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_STATIC_DRAW);

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
        Reset();

        // Set Uniforms.
        m_Shader.Use();
        m_Shader.SetInt("u_Texture", 0);
        m_Shader.SetMat4("u_Transform", m_Transform);
        Shader::Unbind();
    }

    void Transformations::RenderLecture(bool settingsVisible)
    {
        if(settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Transformations");

            if (ImGui::TreeNode("Order of Transformations"))
            {
                //DrawTextCentered("Order of Transformations");
                for (int n = 0; n < IM_ARRAYSIZE(s_Operations); n++)
                {
                    const char* item = s_Operations[n];
                    ImGui::Selectable(item);

                    if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                    {
                        int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                        if (n_next >= 0 && n_next < IM_ARRAYSIZE(s_Operations))
                        {
                            s_Operations[n] = s_Operations[n_next];
                            s_Operations[n_next] = item;
                            ImGui::ResetMouseDragDelta();
                        }
                    }
                }
                ImGui::TreePop();
            }

            ImGui::NewLine();

            ImGui::DragFloat3(LabelPrefix("Position: ").c_str(), &m_Position[0], 0.01f, -1.0f, 1.0f, "%.2f");
            ImGui::DragFloat3(LabelPrefix("Rotation: ").c_str(), &m_Rotation[0], 0.5f, -360.0f, 360.0f, "%.1f");
            ImGui::DragFloat3(LabelPrefix("Scale: ").c_str(), &m_Scale[0], 0.01f, -5.0f, 5.0f, "%.2f");

            ImGui::NewLine();

            if(DrawButtonCentered("Reset")) Reset();
            
            ImGui::End();
        }

        // Bind Texture.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        // Reset Transform.
        m_Transform = glm::mat4(1.0f);
        
        // Apply all the Three Transformations in the user defined order.
        for(int i = 2; i >= 0; i--)
            ApplyOperation(s_Operations[i]);

        // Bind Shader.
        m_Shader.Use();

        // Update Shader Uniforms.
        m_Shader.SetMat4("u_Transform", m_Transform);

        // Bind VAO.
        glBindVertexArray(m_VAO);
        // Draw Indexed.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        Shader::Unbind();
    }

    void Transformations::CloseLecture()
    {
        glDeleteTextures(1, &m_TextureID);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Transformations::LoadTexture(const char * texturePath)
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

    void Transformations::Reset()
    {
        // Default Texture
        LoadTexture("../src/Assets/Textures/GuyJumping.png");
        m_Transform = glm::mat4(1.0f);
        m_Position = glm::vec3(0.0f);
        m_Rotation = glm::vec3(0.0f);
        m_Scale = glm::vec3(1.0f);
        m_Transform = glm::scale(m_Transform, m_Scale);
        s_Operations[0] = "Scale";
        s_Operations[1] = "Rotate";
        s_Operations[2] = "Translate";
    }

    void Transformations::ApplyOperation(const char *operation)
    {
        if(strcmp(operation, "Translate") == 0)
        {
            // Perform Translation.
            m_Transform = glm::translate(m_Transform, m_Position);
        }
        else if(strcmp(operation, "Rotate") == 0)
        {
            // Perform Rotation in X, Y & Z axis.
            m_Transform = glm::rotate(m_Transform, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m_Transform = glm::rotate(m_Transform, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_Transform = glm::rotate(m_Transform, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else
        {
            // Perform Scaling.
            m_Transform = glm::scale(m_Transform, m_Scale);
        }
    }
}