// Link -> https://www.learnopengl.com/Getting-started/Textures

#include "../../../Lectures.h"
#include "../Shader/Shader.h"
#include "../../../../vendor/glad/include/glad.h"

namespace GettingStarted
{
    class TextureLecture : public Lecture
    {
    public:
        TextureLecture();
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible) override;
        virtual void CloseLecture() override;
    private:
        void LoadTexture(const char* texturePath);
        void Reset();
        void SetTextureProperties();
        void SetWrapMode();
        void SetFilterMode();
    private:
        Shader m_Shader;
        uint32_t m_TextureID;
        uint32_t m_VAO, m_VBO, m_EBO;

        float m_Tiling[2] = {1.0f, 1.0f};
        GLenum m_TextureWrapSMode = GL_REPEAT;
        GLenum m_TextureWrapTMode = GL_REPEAT;
        float m_BorderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float m_Color[3] = {1.0f, 1.0f, 1.0f};

        GLenum m_TextureFilterMINMode = GL_NEAREST;
        GLenum m_TextureFilterMAGMode = GL_NEAREST;

        const char* m_WrapModes[4] = { "GL_REPEAT", "GL_MIRRORED_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER" };
        const char* m_MinFilteringModes[6] = { "GL_NEAREST", "GL_LINEAR", "GL_NEAREST_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_NEAREST", "GL_NEAREST_MIPMAP_LINEAR", "GL_LINEAR_MIPMAP_LINEAR"};
        const char* m_MagFilteringModes[2] = { "GL_NEAREST", "GL_LINEAR" };

        const char* m_CurrentWrapModeS = "GL_REPEAT";
        const char* m_CurrentWrapModeT = "GL_REPEAT"; 
        const char* m_CurrentMinFilteringMode = "GL_NEAREST";
        const char* m_CurrentMagFilteringMode = "GL_NEAREST";

        float m_QuadVertices[20] = 
        {
            // Vertex Positions     // Texture Coordinates
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     // Bottom Left
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f,     // Bottom Right
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f,     // Top Right
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f      // Top Left     
        };
        unsigned int m_Indices[6] =
        {
            0, 1, 2,    // First Triangle
            2, 3, 0     // Second Triangle
        };
    };
}
