#include "CameraLecture.h"
#include "Camera.h"
#include "../../../../vendor/glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <stb_image.h>

namespace GettingStarted
{
    CameraLecture::CameraLecture(int width, int height) : 
        m_Shader(PROJECT_DIR"/src/Lectures/GettingStarted/Camera/CameraLecture.vert",
                 PROJECT_DIR"/src/Lectures/GettingStarted/Camera/CameraLecture.frag")
    {
        m_LectureLink = "https://www.learnopengl.com/Getting-started/Camera";
        
        // Initialize default values.
        Reset(width, height);
    }

    void CameraLecture::OpenLecture()
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

    void CameraLecture::RenderLecture(bool settingsVisible, int width, int height)
    {
        Camera* cam = Camera::GetInstance();

        if(settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Camera Settings");
            
            ImGui::DragFloat(LabelPrefix("Camera Speed: ").c_str(), &cam->MovementSpeed, 0.05f, -1000.0f, 1000.0f, "%.2f");
            ImGui::DragFloat(LabelPrefix("Camera Sensitivity: ").c_str(), &cam->MouseSensitivity, 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::Checkbox(LabelPrefix("Freeze Y: ").c_str(), &cam->freezeY);
            ImGui::Checkbox(LabelPrefix("Invert Mouse: ").c_str(), &cam->invertMouse);

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
        m_Width = width;
        m_Height = height;
        float aspect = (float)m_Width / (float)m_Height;

        m_Projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        m_View = cam->GetViewMatrix();
        
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

    void CameraLecture::CloseLecture()
    {
        glDeleteTextures(1, &m_TextureID);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        // Reset Depth Testing to false.
        glDisable(GL_DEPTH_TEST);
    }

    void CameraLecture::LoadTexture(const char * texturePath)
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

    void CameraLecture::Reset(int width, int height)
    {
        // Default Texture
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/GuyJumping.png");
        m_Model = glm::mat4(1.0f);
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;

        // Reset Camera Options

        Camera* cam = Camera::GetInstance();
        cam->Yaw = -90.0f;
        cam->Pitch =  0.0f;
        cam->MovementSpeed =  2.5f;
        cam->MouseSensitivity =  0.1f;
        cam->freezeY = false;
        cam->invertMouse = false;
        cam->Zoom =  45.0f;

        cam->Position = glm::vec3(0.0f, 0.0f, 3.0f);
        cam->Up = glm::vec3(0.0f, 1.0f, 0.0f);
        cam->Front = glm::vec3(0.0f, 0.0f, -1.0f);

        m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, 3.0f));
        m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/(float)m_Height, 0.1f, 100.0f);
    }
}