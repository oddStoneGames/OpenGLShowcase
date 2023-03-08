#include "LightingMaps.h"
#include "../../GettingStarted/Camera/Camera.h"
#include "../../../../vendor/glm/gtc/matrix_transform.hpp"
#include <string>
#include <iostream>
#include <stb_image.h>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

namespace Lighting
{
    LightingMaps::LightingMaps(int width, int height) : 
        m_LightShader(PROJECT_DIR"/src/Lectures/Lighting/LightingMaps/LightCube.vert",
                      PROJECT_DIR"/src/Lectures/Lighting/LightingMaps/LightCube.frag"), 
        m_CubeShader(PROJECT_DIR"/src/Lectures/Lighting/LightingMaps/LightingMaps.vert",
                     PROJECT_DIR"/src/Lectures/Lighting/LightingMaps/LightingMaps.frag"),
        m_LightAmbientColor(0.1f), m_LightDiffuseColor(0.5f), m_LightSpecularColor(1.0f),
        m_MaterialShininess(32.0f)
    {
        m_LectureLink = "https://www.learnopengl.com/Lighting/Lighting-maps";

        // Initialize default values.
        Reset(width, height);
    }

    void LightingMaps::OpenLecture()
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture Coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(m_VAO);
        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
      
        // Generate Texture
        glGenTextures(1, &m_DiffuseTextureID);
        glGenTextures(1, &m_SpecularTextureID);
        glGenTextures(1, &m_EmissionTextureID);

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
        m_CubeShader.SetInt("u_Material.diffuse",  0);
        m_CubeShader.SetInt("u_Material.specular", 1);
        m_CubeShader.SetInt("u_Material.emission", 2);
        m_CubeShader.SetFloat("u_Material.shininess", m_MaterialShininess);

        GettingStarted::Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void LightingMaps::RenderLecture(bool settingsVisible, int width, int height)
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

            ImGui::Text("Diffuse Texture:    ");
            ImGui::SameLine();
            if (ImGui::Button("Choose  Diffuse Texture")) ChooseCustomTexture(m_DiffuseTextureID);

            ImGui::Text("Specular Texture:   ");
            ImGui::SameLine();
            if (ImGui::Button("Choose Specular Texture")) ChooseCustomTexture(m_SpecularTextureID);

            ImGui::Text("Emission Texture:   ");
            ImGui::SameLine();
            if (ImGui::Button("Choose Emission Texture")) ChooseCustomTexture(m_EmissionTextureID);

            ImGui::DragFloat(LabelPrefix("Shininess: ").c_str(), &m_MaterialShininess, 0.05f, 0.0f, 1000.0f, "%.01f");
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

        // Bind Diffuse Map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_DiffuseTextureID);

        // Bind Specular Map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_SpecularTextureID);

        // Bind Emission Map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_EmissionTextureID);

        m_CubeShader.Use();
        m_CubeShader.SetMat4("u_Model", m_CubeModel);
        m_CubeShader.SetMat4("u_View", m_View);
        m_CubeShader.SetMat4("u_Projection", m_Projection);
        m_CubeShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_CubeShader.SetFloat3("u_Light.position", m_LightPosition);
        m_CubeShader.SetFloat3("u_Light.ambient", m_LightAmbientColor);
        m_CubeShader.SetFloat3("u_Light.diffuse", m_LightDiffuseColor);
        m_CubeShader.SetFloat3("u_Light.specular", m_LightSpecularColor);
        m_CubeShader.SetFloat("u_Material.shininess", m_MaterialShininess);

        // Draw Indexed.
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        GettingStarted::Shader::Unbind();
    }

    void LightingMaps::CloseLecture()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteVertexArrays(1, &m_CubeVAO);
        glDeleteBuffers(1, &m_VBO);

        // Reset Depth Testing to false.
        glDisable(GL_DEPTH_TEST);
    }

    void LightingMaps::ChooseCustomTexture(uint32_t textureID)
    {
        // Open a Popup to Select File.
        char filename[FILENAME_MAX] = "";

#ifdef WIN32
        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = NULL;
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "";

        std::string fileNameStr;
        if (GetOpenFileName(&ofn))
        {
            fileNameStr = filename;
            // Load Image.
            LoadTexture(fileNameStr.c_str(), textureID);
        }
#else
        // Linux File open dialog
        FILE* f = popen("zenity --file-selection", "r");
        fgets(filename, FILENAME_MAX, f);
        std::string filePath = filename;
        // Replace the Null Char with Espace Backslashes otherwise stb_load won't be able to open the file.
        filePath.replace(filePath.length() - 1, 1, '\0', '\\');
        // Load Image.
        LoadTexture(filePath.c_str(), textureID);
#endif // WIN32
    }

    void LightingMaps::LoadTexture(const char* texturePath, uint32_t textureID)
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

    void LightingMaps::Reset(int width, int height)
    {
        // Default Matrices
        m_Model = glm::mat4(1.0f);
        m_CubeModel = glm::mat4(1.0f);
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;

        // Default Textures
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/diffuse.png", m_DiffuseTextureID);
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/specular.png", m_SpecularTextureID);
        LoadTexture(PROJECT_DIR"/src/Assets/Textures/emission.png", m_EmissionTextureID);

        // Reset Options
        m_LightPosition = glm::vec3(0.0f);
        m_LightAmbientColor = glm::vec3(0.1f);
        m_LightDiffuseColor = glm::vec3(0.5f);
        m_LightSpecularColor = glm::vec3(1.0f);
        m_MaterialShininess = 32.0f;

        m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, 3.0f));
        m_Model = glm::scale(m_Model, glm::vec3(0.5f));
        m_CubeModel = glm::translate(m_CubeModel, glm::vec3(0.7f, -0.3f, -1.1f));
        m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/(float)m_Height, 0.1f, 100.0f);
    }
}