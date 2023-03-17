#include "DepthTesting.h"
#include "../../GettingStarted/Camera/Camera.h"
#include "../../../../vendor/glm/gtc/matrix_transform.hpp"
#include <string>
#include <iostream>
#include <stb_image.h>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

namespace AdvancedOpenGL
{
    DepthTesting::DepthTesting(int width, int height) : 
        m_Shader(PROJECT_DIR"/src/Lectures/AdvancedOpenGL/DepthTesting/DepthTesting.vert",
                     PROJECT_DIR"/src/Lectures/AdvancedOpenGL/DepthTesting/DepthTesting.frag")
    {
        m_LectureLink = "https://www.learnopengl.com/Advanced-OpenGL/Depth-testing";

        // Initialize default values.
        Reset(width, height);
    }

    void DepthTesting::OpenLecture()
    {
        // Generate VAO, VBO & setup vertex attribute pointers to pass the data to GPU correctly.
        glGenVertexArrays(1, &m_PlaneVAO);
        glGenVertexArrays(1, &m_CubeVAO);
        glGenBuffers(1, &m_PlaneVBO);
        glGenBuffers(1, &m_CubeVBO);

        // Pass Cube Data
        glBindVertexArray(m_CubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
        
        // Pass Buffer Data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_CubeVertices), m_CubeVertices, GL_STATIC_DRAW);

        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture Coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Pass Plane Data
        glBindVertexArray(m_PlaneVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVBO);
        
        // Pass Buffer Data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_PlaneVertices), m_PlaneVertices, GL_STATIC_DRAW);

        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture Coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
      
        // Generate Texture
        glGenTextures(1, &m_DiffuseTextureID);
        glGenTextures(1, &m_SpecularTextureID);
        glGenTextures(1, &m_PlaneDiffuseTextureID);

        // Reset Options.
        Reset(m_Width, m_Height);

        // Set Uniforms.
        m_Shader.Use();
        m_Shader.SetMat4("u_Model", m_Model);
        m_Shader.SetMat4("u_View", m_View); 
        m_Shader.SetMat4("u_Projection", m_Projection);
        m_Shader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_Shader.SetFloat3("u_DirectionalLight.direction", m_DirectionalLight.direction);
        m_Shader.SetFloat3("u_DirectionalLight.ambient",   m_DirectionalLight.ambient);
        m_Shader.SetFloat3("u_DirectionalLight.diffuse",   m_DirectionalLight.diffuse);
        m_Shader.SetFloat3("u_DirectionalLight.specular",  m_DirectionalLight.specular);

        m_Shader.SetInt("u_Material.diffuse",  0);
        m_Shader.SetInt("u_Material.specular", 1);
        m_Shader.SetFloat("u_Material.shininess", 32.0f);

        m_Shader.SetFloat("u_Near", 0.1f);
        m_Shader.SetFloat("u_Far", 100.0f);

        m_Shader.SetInt("u_VisualizeDepth", m_VisualizeDepth);
        m_Shader.SetInt("u_LinearizeDepth", m_LinearizeDepth);

        GettingStarted::Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void DepthTesting::RenderLecture(bool settingsVisible, int width, int height)
    {
        if (settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Depth Test Settings");

            if (ImGui::Checkbox(LabelPrefix("Enable Depth Testing: ").c_str(), &m_DepthTestEnabled))
            {
                if (m_DepthTestEnabled) glEnable(GL_DEPTH_TEST);
                else glDisable(GL_DEPTH_TEST);
            }

            if (m_DepthTestEnabled)
            {
                ImGui::Checkbox(LabelPrefix("Visualize Depth: ").c_str(), &m_VisualizeDepth);
                ImGui::Checkbox(LabelPrefix("Linearize Depth: ").c_str(), &m_LinearizeDepth);

                // Texture Wrap S
                if (ImGui::BeginCombo(LabelPrefix("Depth Test Function: ").c_str(), s_CurrentDepthTestFunc))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(s_DepthTestFuncs); n++)
                    {
                        bool is_selected = (s_CurrentDepthTestFunc == s_DepthTestFuncs[n]);
                        if (ImGui::Selectable(s_DepthTestFuncs[n], is_selected))
                            s_CurrentDepthTestFunc = s_DepthTestFuncs[n];
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                SetDepthFunc();
                glDepthFunc(m_CurrentDepthTestFunc);
            }

            ImGui::NewLine();

            if (DrawButtonCentered("Reset")) Reset(width, height);

            ImGui::End();
        }

        GettingStarted::Camera* cam = GettingStarted::Camera::GetInstance();

        // Reset Matrices.
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;
        float aspect = (float)m_Width/(float)m_Height;

        m_Projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        m_View = cam->GetViewMatrix();

        // Bind Cube VAO.
        glBindVertexArray(m_CubeVAO);

        // Bind Diffuse Map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_DiffuseTextureID);

        // Bind Specular Map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_SpecularTextureID);

        m_Shader.Use();
        m_Shader.SetMat4("u_View", m_View);
        m_Shader.SetMat4("u_Projection", m_Projection);
        m_Shader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_Shader.SetFloat3("u_DirectionalLight.direction", m_DirectionalLight.direction);
        m_Shader.SetFloat3("u_DirectionalLight.ambient", m_DirectionalLight.ambient);
        m_Shader.SetFloat3("u_DirectionalLight.diffuse", m_DirectionalLight.diffuse);
        m_Shader.SetFloat3("u_DirectionalLight.specular", m_DirectionalLight.specular);

        m_Shader.SetInt("u_Material.diffuse", 0);
        m_Shader.SetInt("u_Material.specular", 1);
        m_Shader.SetFloat("u_Material.shininess", 32.0f);
        m_Shader.SetInt("u_VisualizeDepth", m_VisualizeDepth);
        m_Shader.SetInt("u_LinearizeDepth", m_LinearizeDepth);

        for (unsigned int i = 0; i < 2; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            m_Model = glm::mat4(1.0f);
            m_Model = glm::translate(m_Model, m_CubePositions[i]);
            float angle = 20.0f * i;
            m_Model = glm::rotate(m_Model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
            m_Shader.SetMat4("u_Model", m_Model);

            // Draw Triangles.
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Bind Plane VAO
        glBindVertexArray(m_PlaneVAO);

        m_Model = glm::mat4(1.0f);
        m_Model = glm::scale(m_Model, glm::vec3(10.0f));
        m_Model = glm::translate(m_Model, m_PlanePosition);

        // Bind Diffuse Map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_PlaneDiffuseTextureID);

        // Unbind Specular Map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_Shader.SetMat4("u_Model", m_Model);

        // Draw Triangles.
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        GettingStarted::Shader::Unbind();
    }

    void DepthTesting::CloseLecture()
    {
        glDeleteVertexArrays(1, &m_CubeVAO);
        glDeleteVertexArrays(1, &m_PlaneVAO);
        glDeleteBuffers(1, &m_CubeVBO);
        glDeleteBuffers(1, &m_PlaneVBO);
        glDeleteTextures(1, &m_DiffuseTextureID);
        glDeleteTextures(1, &m_SpecularTextureID);
        glDeleteTextures(1, &m_PlaneDiffuseTextureID);

        // Reset Depth Testing to false.
        glDepthFunc(GL_LESS);
        glDisable(GL_DEPTH_TEST);
    }

    void DepthTesting::SetDepthFunc()
    {
        if (strcmp(s_CurrentDepthTestFunc, "GL_ALWAYS") == 0)
            m_CurrentDepthTestFunc = GL_ALWAYS;
        else if (strcmp(s_CurrentDepthTestFunc, "GL_NEVER") == 0)
            m_CurrentDepthTestFunc = GL_NEVER;
        else if (strcmp(s_CurrentDepthTestFunc, "GL_LESS") == 0)
            m_CurrentDepthTestFunc = GL_LESS;
        else if (strcmp(s_CurrentDepthTestFunc, "GL_EQUAL") == 0)
            m_CurrentDepthTestFunc = GL_EQUAL;
        else if (strcmp(s_CurrentDepthTestFunc, "GL_LEQUAL") == 0)
            m_CurrentDepthTestFunc = GL_LEQUAL;
        else if (strcmp(s_CurrentDepthTestFunc, "GL_GREATER") == 0)
            m_CurrentDepthTestFunc = GL_GREATER;
        else if (strcmp(s_CurrentDepthTestFunc, "GL_NOTEQUAL") == 0)
            m_CurrentDepthTestFunc = GL_NOTEQUAL;
        else
            m_CurrentDepthTestFunc = GL_GEQUAL;
    }

    void DepthTesting::LoadTexture(const char* texturePath, uint32_t textureID)
    {
        // Bind Texture.
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
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
            if (stbi_failure_reason())
                std::cout << stbi_failure_reason();
            std::cout << "\nFailed to load texture" << std::endl;
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void DepthTesting::Reset(int width, int height)
    {
        // Default Matrices
        m_Model = glm::mat4(1.0f);
        m_Model = glm::mat4(1.0f);
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;

        // Default Textures
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/diffuse.png", m_DiffuseTextureID);
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/specular.png", m_SpecularTextureID);
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/GuyJumping.png", m_PlaneDiffuseTextureID);

        // Reset Light Values.
        m_DirectionalLight.direction = glm::vec3(0.2f, -0.8f, -0.6f);
        m_DirectionalLight.ambient = glm::vec3(0.1f);
        m_DirectionalLight.diffuse = glm::vec3(0.5f);
        m_DirectionalLight.specular = glm::vec3(1.0f);

        m_DepthTestEnabled = true;
        s_CurrentDepthTestFunc = "GL_LESS";
        m_CurrentDepthTestFunc = GL_LESS;
        glDepthFunc(m_CurrentDepthTestFunc);
        m_VisualizeDepth = false;
        m_LinearizeDepth = false;

        m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, 3.0f));
        m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/(float)m_Height, 0.1f, 100.0f);
    }
}