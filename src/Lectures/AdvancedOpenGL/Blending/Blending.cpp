#include "Blending.h"
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
    Blending::Blending(int width, int height) : 
        m_ModelShader(PROJECT_DIR"/src/Lectures/AdvancedOpenGL/Blending/Model.vert",
                      PROJECT_DIR"/src/Lectures/AdvancedOpenGL/Blending/Model.frag"),
        m_WindowShader(PROJECT_DIR"/src/Lectures/AdvancedOpenGL/Blending/Window.vert",
                       PROJECT_DIR"/src/Lectures/AdvancedOpenGL/Blending/Window.frag"),
        m_Model()
    {
        m_LectureLink = "https://www.learnopengl.com/Advanced-OpenGL/Blending";

        // Initialize default values.
        Reset(width, height);
    }

    void Blending::OpenLecture()
    {
        // Reset Options.
        Reset(m_Width, m_Height);

        // Create Window VAO.
        glGenVertexArrays(1, &m_WindowVAO);
        glBindVertexArray(m_WindowVAO);
        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_PlaneVertices), m_PlaneVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        // Load Window Texture.
        glGenTextures(1, &m_WindowTextureID);
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/window.png", m_WindowTextureID);

        // Load Model
        m_Model.LoadModel(PROJECT_DIR"/src/Assets/Models/Pangong/Lake.obj");

        // Set Uniforms. 
        m_ModelShader.Use();
        m_ModelShader.SetMat4("u_Model", m_ModelMatrix);
        m_ModelShader.SetMat4("u_View", m_ViewMatrix); 
        m_ModelShader.SetMat4("u_Projection", m_ProjectionMatrix);
        m_ModelShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_ModelShader.SetFloat3("u_DirectionalLight.direction",  m_DirectionalLight.direction);
        m_ModelShader.SetFloat3("u_DirectionalLight.ambient",   m_DirectionalLight.ambient);
        m_ModelShader.SetFloat3("u_DirectionalLight.diffuse",   m_DirectionalLight.diffuse);
        m_ModelShader.SetFloat3("u_DirectionalLight.specular",  m_DirectionalLight.specular);

        m_ModelShader.SetFloat3("u_Material.color", m_ModelMaterial.color);
        m_ModelShader.SetFloat("u_Material.specularStrength", m_ModelMaterial.specularStrength);
        m_ModelShader.SetFloat("u_Material.shininess", m_ModelMaterial.shininess);

        m_WindowShader.Use();
        m_WindowShader.SetInt("texture1", 0);

        GettingStarted::Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void Blending::RenderLecture(bool settingsVisible, int width, int height)
    {
        if (settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Blending Lecture Settings");

            ImGui::NewLine();

            ImGui::DragFloat3(LabelPrefix("Light Direction: ").c_str(), &m_DirectionalLight.direction[0], 0.01f, -1.0f, 1.0f, "%.2f");
            m_DirectionalLight.direction = glm::normalize(m_DirectionalLight.direction);
            ImGui::NewLine();

            if (ImGui::Checkbox("Enable Blending: ", &m_BlendingEnabled))
            {
                if (m_BlendingEnabled) glEnable(GL_BLEND);
                else glDisable(GL_BLEND);
            }

            if (m_BlendingEnabled)
            {
                // Blending Function
                DrawTextCentered("Blending Function");
                ImGui::NewLine();

                // Source Factor
                if (ImGui::BeginCombo(LabelPrefix("Source Factor: ").c_str(), s_CurrentBlendingSourceFactor))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(s_BlendFuncs); n++)
                    {
                        bool is_selected = (s_CurrentBlendingSourceFactor == s_BlendFuncs[n]);
                        if (ImGui::Selectable(s_BlendFuncs[n], is_selected))
                        {
                            s_CurrentBlendingSourceFactor = s_BlendFuncs[n];
                            // Update Source Blending Factor.
                            m_CurrentBlendingSourceFactor = UpdateBlendingFactor(s_CurrentBlendingSourceFactor);
                            
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                // Destination Factor
                if (ImGui::BeginCombo(LabelPrefix("Destination Factor: ").c_str(), s_CurrentBlendingDestinationFactor))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(s_BlendFuncs); n++)
                    {
                        bool is_selected = (s_CurrentBlendingDestinationFactor == s_BlendFuncs[n]);
                        if (ImGui::Selectable(s_BlendFuncs[n], is_selected))
                        {
                            s_CurrentBlendingDestinationFactor = s_BlendFuncs[n];
                            // Update Destination Blending Factor.
                            m_CurrentBlendingDestinationFactor = UpdateBlendingFactor(s_CurrentBlendingDestinationFactor);
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::NewLine();

                if (s_CurrentBlendingSourceFactor == s_BlendFuncs[10] || s_CurrentBlendingSourceFactor == s_BlendFuncs[11] ||
                    s_CurrentBlendingSourceFactor == s_BlendFuncs[12] || s_CurrentBlendingSourceFactor == s_BlendFuncs[13] ||
                    s_CurrentBlendingDestinationFactor == s_BlendFuncs[10] || s_CurrentBlendingDestinationFactor == s_BlendFuncs[11] ||
                    s_CurrentBlendingDestinationFactor == s_BlendFuncs[12] || s_CurrentBlendingDestinationFactor == s_BlendFuncs[13])
                {
                    // Show GL Blend Color.
                    if(ImGui::ColorEdit4("GL_BLEND_COLOR: ", &m_GLBlendColor[0]))
                        // Send Changes to OpenGL.
                        UpdateBlendingAttributes();

                    // Store that we are using GL BLEND COLOR.
                    m_GLBlendColorEnabled = true;
                }
                else
                {
                    m_GLBlendColorEnabled = false;
                }

                // Blend Equation
                if (ImGui::BeginCombo(LabelPrefix("Blend Equation: ").c_str(), s_CurrentBlendingEquation))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(s_BlendEquations); n++)
                    {
                        bool is_selected = (s_CurrentBlendingEquation == s_BlendEquations[n]);
                        if (ImGui::Selectable(s_BlendEquations[n], is_selected))
                        {
                            s_CurrentBlendingEquation = s_BlendEquations[n];
                            // Send Changes to OpenGL.
                            UpdateBlendingAttributes();
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::NewLine();
            }

            if (DrawButtonCentered("Reset")) Reset(width, height);

            ImGui::End();
        }

        GettingStarted::Camera* cam = GettingStarted::Camera::GetInstance();

        // Reset Matrices.
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionMatrix = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;
        float aspect = (float)m_Width/(float)m_Height;

        m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        m_ViewMatrix = cam->GetViewMatrix();

        glm::vec3 camPosition = cam->Position;

        // Draw Model
        m_ModelShader.Use();

        // Update Shader Uniforms.
        m_ModelShader.SetMat4("u_View", m_ViewMatrix);
        m_ModelShader.SetMat4("u_Projection", m_ProjectionMatrix);
        m_ModelShader.SetFloat3("u_ViewPos", camPosition);

        m_ModelMatrix = glm::mat4(1.0f);

        m_ModelShader.SetMat4("u_Model", m_ModelMatrix);
        m_ModelShader.SetFloat3("u_DirectionalLight.direction", m_DirectionalLight.direction);

        m_Model.Draw(m_ModelShader);

        glBindVertexArray(m_WindowVAO);
        
        m_WindowShader.Use();
        m_WindowShader.SetMat4("u_View", m_ViewMatrix);
        m_WindowShader.SetMat4("u_Projection", m_ProjectionMatrix);

        glBindTexture(GL_TEXTURE_2D, m_WindowTextureID);
        glActiveTexture(GL_TEXTURE0);

        // Draw Windows Sorted from Farthest to Closest to the camera.
        for (unsigned int i = 0; i < 4; i++)
        {
            float distance = glm::length(camPosition - m_WindowPositions[i]);
            m_SortedWindowPositions[distance] = m_WindowPositions[i];
        }

        for (std::map<float, glm::vec3>::reverse_iterator it = m_SortedWindowPositions.rbegin(); it != m_SortedWindowPositions.rend(); ++it)
        {
            m_ModelMatrix = glm::mat4(1.0f);
            m_ModelMatrix = glm::translate(m_ModelMatrix, it->second);
            m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(0.5f));
            m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            m_WindowShader.SetMat4("u_Model", m_ModelMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        GettingStarted::Shader::Unbind();
        glBindVertexArray(0);
    }

    void Blending::CloseLecture()
    {
        // Free memory.
        m_Model.Free();
        glDeleteVertexArrays(1, &m_WindowVAO);
        glDeleteTextures(1, &m_WindowTextureID);

        // Disable Depth Testing & Blending.
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }

    void Blending::Reset(int width, int height)
    {
        // Default Matrices
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionMatrix = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;
        m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, 3.0f));
        m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f);

        // Reset Light Values.
        m_DirectionalLight.direction = glm::vec3(0.15f, 0.91f, 0.38f);
        m_DirectionalLight.ambient = glm::vec3(0.23f);
        m_DirectionalLight.diffuse = glm::vec3(0.5f);
        m_DirectionalLight.specular = glm::vec3(1.0f);
        m_ModelMaterial.color = glm::vec3(1.0f);
        m_ModelMaterial.specularStrength = 1.0f;
        m_ModelMaterial.shininess = 32.0f;
        m_ModelMatrix = glm::mat4(1.0f);
        
        m_BlendingEnabled = true;
        s_CurrentBlendingSourceFactor = s_BlendFuncs[6];
        s_CurrentBlendingDestinationFactor = s_BlendFuncs[7];

        m_CurrentBlendingSourceFactor = GL_SRC_ALPHA;
        m_CurrentBlendingDestinationFactor = GL_ONE_MINUS_SRC_ALPHA;

        s_CurrentBlendingEquation = s_BlendEquations[0];
        m_CurrentBlendEquation = GL_FUNC_ADD;

        m_GLBlendColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        // Enable Blending.
        glEnable(GL_BLEND);
        glBlendFunc(m_CurrentBlendingSourceFactor, m_CurrentBlendingDestinationFactor);
        if (m_GLBlendColorEnabled)
            glBlendColor(m_GLBlendColor.r, m_GLBlendColor.g, m_GLBlendColor.b, m_GLBlendColor.a);
        glBlendEquation(m_CurrentBlendEquation);
    }

    GLenum Blending::UpdateBlendingFactor(const char* blendFactor)
    {
        GLenum factor = GL_NONE;

        if (strcmp(blendFactor, "GL_ZERO") == 0)
            factor = GL_ZERO;
        else if (strcmp(blendFactor, "GL_ONE") == 0)
            factor = GL_ONE;
        else if (strcmp(blendFactor, "GL_SRC_COLOR") == 0)
            factor = GL_SRC_COLOR;
        else if (strcmp(blendFactor, "GL_ONE_MINUS_SRC_COLOR") == 0)
            factor = GL_ONE_MINUS_SRC_COLOR;
        else if (strcmp(blendFactor, "GL_DST_COLOR") == 0)
            factor = GL_DST_COLOR;
        else if (strcmp(blendFactor, "GL_ONE_MINUS_DST_COLOR") == 0)
            factor = GL_ONE_MINUS_DST_COLOR;
        else if (strcmp(blendFactor, "GL_SRC_ALPHA") == 0)
            factor = GL_SRC_ALPHA;
        else if (strcmp(blendFactor, "GL_ONE_MINUS_SRC_ALPHA") == 0)
            factor = GL_ONE_MINUS_SRC_ALPHA;
        else if (strcmp(blendFactor, "GL_DST_ALPHA") == 0)
            factor = GL_DST_ALPHA;
        else if (strcmp(blendFactor, "GL_ONE_MINUS_DST_ALPHA") == 0)
            factor = GL_ONE_MINUS_DST_ALPHA;
        else if (strcmp(blendFactor, "GL_CONSTANT_COLOR") == 0)
            factor = GL_CONSTANT_COLOR;
        else if (strcmp(blendFactor, "GL_ONE_MINUS_CONSTANT_COLOR") == 0)
            factor = GL_ONE_MINUS_CONSTANT_COLOR;
        else if (strcmp(blendFactor, "GL_CONSTANT_ALPHA") == 0)
            factor = GL_CONSTANT_ALPHA;
        else if (strcmp(blendFactor, "GL_ONE_MINUS_CONSTANT_ALPHA") == 0)
            factor = GL_ONE_MINUS_CONSTANT_ALPHA;

        return factor;
    }

    void Blending::UpdateBlendingAttributes()
    {
        // Update Blend Equation.
        if (strcmp(s_CurrentBlendingEquation, "GL_FUNC_ADD") == 0)
            m_CurrentBlendEquation = GL_FUNC_ADD;
        else if (strcmp(s_CurrentBlendingEquation, "GL_FUNC_SUBTRACT") == 0)
            m_CurrentBlendEquation = GL_FUNC_SUBTRACT;
        else if (strcmp(s_CurrentBlendingEquation, "GL_FUNC_REVERSE_SUBTRACT") == 0)
            m_CurrentBlendEquation = GL_FUNC_REVERSE_SUBTRACT;
        else if (strcmp(s_CurrentBlendingEquation, "GL_MIN") == 0)
            m_CurrentBlendEquation = GL_MIN;
        else if (strcmp(s_CurrentBlendingEquation, "GL_MAX") == 0)
            m_CurrentBlendEquation = GL_MAX;
        else
            m_CurrentBlendEquation = GL_FUNC_ADD;

        glBlendFunc(m_CurrentBlendingSourceFactor, m_CurrentBlendingDestinationFactor);
        if (m_GLBlendColorEnabled)
            glBlendColor(m_GLBlendColor.r, m_GLBlendColor.g, m_GLBlendColor.b, m_GLBlendColor.a);
        glBlendEquation(m_CurrentBlendEquation);
    }

    void Blending::LoadTexture(const char* texturePath, uint32_t& textureID)
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
            glTexImage2D(GL_TEXTURE_2D, 0, colorChannels, width, height, 0, colorChannels, GL_UNSIGNED_BYTE, data);
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
}
