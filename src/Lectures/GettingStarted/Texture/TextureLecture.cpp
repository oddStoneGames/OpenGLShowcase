#include "TextureLecture.h"
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace GettingStarted
{
    TextureLecture::TextureLecture() : 
        //using '../' because the current working directory is bin.
        m_Shader("../src/Lectures/GettingStarted/Texture/Texture.vert",
                 "../src/Lectures/GettingStarted/Texture/Texture.frag")
    {
        m_LectureLink = "xdg-open https://www.learnopengl.com/Getting-started/Textures";
    }

    void TextureLecture::OpenLecture()
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
        m_Shader.SetFloat2("u_Tiling", 1.0f, 1.0f);
        m_Shader.SetFloat3("u_Color", 1.0f, 1.0f, 1.0f);
        Shader::Unbind();
    }

    void TextureLecture::RenderLecture(bool settingsVisible)
    {
        if(settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Texture");
            if(DrawButtonCentered("Choose Custom Image"))
            {
                // Open a Popup to Select File.
                char filename[FILENAME_MAX];
                FILE *f = popen("zenity --file-selection", "r");
                fgets(filename, FILENAME_MAX, f);
                std::string filePath = filename;
                // Replace the Null Char with Espace Backslashes otherwise stb_load won't be able to open the file.
                filePath.replace(filePath.length() - 1, 1, '\0', '\\');
                // Load Image.
                LoadTexture(filePath.c_str());
            }

            ImGui::DragFloat2(LabelPrefix("Tiling: ").c_str(), m_Tiling, 0.1f, -10.0f, 10.0f, "%.1f");
            ImGui::ColorEdit3(LabelPrefix("Color: ").c_str(), m_Color);

            // Texture Wrap S
            if (ImGui::BeginCombo(LabelPrefix("S Wrap Mode: ").c_str(), m_CurrentWrapModeS))
            {
                for (int n = 0; n < IM_ARRAYSIZE(m_WrapModes); n++)
                {
                    bool is_selected = (m_CurrentWrapModeS == m_WrapModes[n]);
                    if (ImGui::Selectable(m_WrapModes[n], is_selected))
                        m_CurrentWrapModeS = m_WrapModes[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Texture Wrap T
            if (ImGui::BeginCombo(LabelPrefix("T Wrap Mode: ").c_str(), m_CurrentWrapModeT))
            {
                for (int n = 0; n < IM_ARRAYSIZE(m_WrapModes); n++)
                {
                    bool is_selected = (m_CurrentWrapModeT == m_WrapModes[n]);
                    if (ImGui::Selectable(m_WrapModes[n], is_selected))
                        m_CurrentWrapModeT = m_WrapModes[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Texture Filtering Min
            if (ImGui::BeginCombo(LabelPrefix("Minifying Filter Mode: ").c_str(), m_CurrentMinFilteringMode))
            {
                for (int n = 0; n < IM_ARRAYSIZE(m_MinFilteringModes); n++)
                {
                    bool is_selected = (m_CurrentMinFilteringMode == m_MinFilteringModes[n]);
                    if (ImGui::Selectable(m_MinFilteringModes[n], is_selected))
                        m_CurrentMinFilteringMode = m_MinFilteringModes[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Texture Filtering Mag
            if (ImGui::BeginCombo(LabelPrefix("Magnifying Filter Mode: ").c_str(), m_CurrentMagFilteringMode))
            {
                for (int n = 0; n < IM_ARRAYSIZE(m_MagFilteringModes); n++)
                {
                    bool is_selected = (m_CurrentMagFilteringMode == m_MagFilteringModes[n]);
                    if (ImGui::Selectable(m_MagFilteringModes[n], is_selected))
                        m_CurrentMagFilteringMode = m_MagFilteringModes[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if(m_TextureWrapSMode == GL_CLAMP_TO_BORDER || m_TextureWrapTMode == GL_CLAMP_TO_BORDER)
                ImGui::ColorEdit3(LabelPrefix("Border Color: ").c_str(), m_BorderColor);

            if(DrawButtonCentered("Reset")) Reset();
            
            ImGui::End();
        }

        // Bind Texture.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        // Update Texture Properties.
        SetTextureProperties();
        // Bind Shader.
        m_Shader.Use();
        // Update Shader Uniforms.
        m_Shader.SetFloat2("u_Tiling", m_Tiling[0], m_Tiling[1]);
        m_Shader.SetFloat3("u_Color", m_Color[0], m_Color[1], m_Color[2]);
        // Bind VAO.
        glBindVertexArray(m_VAO);
        // Draw Indexed.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        Shader::Unbind();
    }

    void TextureLecture::CloseLecture()
    {
        glDeleteTextures(1, &m_TextureID);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void TextureLecture::LoadTexture(const char * texturePath)
    {
        // Generate Texture.
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        
        // Set Texture Properties.
        SetTextureProperties();

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

    void TextureLecture::Reset()
    {
        // Reset Setting Options.
        m_TextureWrapSMode = GL_REPEAT;
        m_TextureWrapTMode = GL_REPEAT;
        m_TextureFilterMINMode = GL_NEAREST;
        m_TextureFilterMAGMode = GL_NEAREST;
        
        // Default Texture
        LoadTexture("../src/Assets/Textures/sampleTexture.png");

        for(int i = 0; i < 3; i++)
        {
            if(i < 2) m_Tiling[i] = 1.0f;
            m_BorderColor[i] = 1.0f;
            m_Color[i] = 1.0f;
        }
    }

    void TextureLecture::SetTextureProperties()
    {
        // Set Texture Wrap & Filter Mode based on selection.
        SetWrapMode();
        SetFilterMode();

        // Set Border Color.
        if(m_TextureWrapSMode == GL_CLAMP_TO_BORDER || m_TextureWrapTMode == GL_CLAMP_TO_BORDER)
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &m_BorderColor[0]); 

        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureWrapSMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureWrapTMode); 

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureFilterMINMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureFilterMAGMode);
    }

    /// @brief Updates the GLenum according to the user given value.
    void TextureLecture::SetWrapMode()
    {
        if(strcmp(m_CurrentWrapModeS, "GL_REPEAT") == 0)
            m_TextureWrapSMode = GL_REPEAT;
        else if(strcmp(m_CurrentWrapModeS, "GL_MIRRORED_REPEAT") == 0)
            m_TextureWrapSMode = GL_MIRRORED_REPEAT;
        else if(strcmp(m_CurrentWrapModeS, "GL_CLAMP_TO_EDGE") == 0)
            m_TextureWrapSMode = GL_CLAMP_TO_EDGE;
        else if(strcmp(m_CurrentWrapModeS, "GL_CLAMP_TO_BORDER") == 0)
            m_TextureWrapSMode = GL_CLAMP_TO_BORDER;
        else
            m_TextureWrapSMode = GL_REPEAT;

        if(strcmp(m_CurrentWrapModeT, "GL_REPEAT") == 0)
            m_TextureWrapTMode = GL_REPEAT;
        else if(strcmp(m_CurrentWrapModeT, "GL_MIRRORED_REPEAT") == 0)
            m_TextureWrapTMode = GL_MIRRORED_REPEAT;
        else if(strcmp(m_CurrentWrapModeT, "GL_CLAMP_TO_EDGE") == 0)
            m_TextureWrapTMode = GL_CLAMP_TO_EDGE;
        else if(strcmp(m_CurrentWrapModeT, "GL_CLAMP_TO_BORDER") == 0)
            m_TextureWrapTMode = GL_CLAMP_TO_BORDER;
        else
            m_TextureWrapTMode = GL_REPEAT;
    }

    /// @brief Updates the GLenum according to the user given value.
    void TextureLecture::SetFilterMode()
    {
        if(strcmp(m_CurrentMinFilteringMode, "GL_NEAREST") == 0)
            m_TextureFilterMINMode = GL_NEAREST;
        else if(strcmp(m_CurrentMinFilteringMode, "GL_LINEAR") == 0)
            m_TextureFilterMINMode = GL_LINEAR;
        else if(strcmp(m_CurrentMinFilteringMode, "GL_NEAREST_MIPMAP_NEAREST") == 0)
            m_TextureFilterMINMode = GL_NEAREST_MIPMAP_NEAREST;
        else if(strcmp(m_CurrentMinFilteringMode, "GL_LINEAR_MIPMAP_NEAREST") == 0)
            m_TextureFilterMINMode = GL_LINEAR_MIPMAP_NEAREST;
        else if(strcmp(m_CurrentMinFilteringMode, "GL_NEAREST_MIPMAP_LINEAR") == 0)
            m_TextureFilterMINMode = GL_NEAREST_MIPMAP_LINEAR;
        else if(strcmp(m_CurrentMinFilteringMode, "GL_LINEAR_MIPMAP_LINEAR") == 0)
            m_TextureFilterMINMode = GL_LINEAR_MIPMAP_LINEAR;
        else
            m_TextureFilterMINMode = GL_NEAREST;

        if(strcmp(m_CurrentMagFilteringMode, "GL_NEAREST") == 0)
            m_TextureFilterMAGMode = GL_NEAREST;
        else if(strcmp(m_CurrentMinFilteringMode, "GL_LINEAR") == 0)
            m_TextureFilterMAGMode = GL_LINEAR;
        else
            m_TextureFilterMAGMode = GL_NEAREST;
        
    }
}