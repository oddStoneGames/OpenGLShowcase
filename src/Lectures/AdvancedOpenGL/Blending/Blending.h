// Link -> https://www.learnopengl.com/Advanced-OpenGL/Blending

#include "../../GettingStarted/Shader/Shader.h"
#include "../../../Lectures.h"
#include "../../../../vendor/glad/include/glad.h"
#include "../../../../vendor/glm/glm.hpp"
#include "../../ModelLoading/Model.h"
#include <map>

namespace AdvancedOpenGL
{
    class Blending : public Lecture
    {
    public:
        Blending(int width, int height);
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible, int width, int height) override;
        virtual void CloseLecture() override;
    private:
        void Reset(int width, int height);
        GLenum UpdateBlendingFactor(const char* blendFactors);
        void LoadTexture(const char* texturePath, uint32_t& textureID);
        void UpdateBlendingAttributes();
    private:
        GettingStarted::Shader m_ModelShader, m_WindowShader;
        ModelLoading::Model m_Model;

        glm::mat4 m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix;
        uint32_t m_Width, m_Height;

        uint32_t m_WindowVAO;
        uint32_t m_WindowTextureID;

        bool m_BlendingEnabled = true;
        const char* s_BlendFuncs[14] = { "GL_ZERO", "GL_ONE", "GL_SRC_COLOR", "GL_ONE_MINUS_SRC_COLOR", "GL_DST_COLOR", "GL_ONE_MINUS_DST_COLOR", "GL_SRC_ALPHA", "GL_ONE_MINUS_SRC_ALPHA",
                                        "GL_DST_ALPHA", "GL_ONE_MINUS_DST_ALPHA", "GL_CONSTANT_COLOR", "GL_ONE_MINUS_CONSTANT_COLOR", "GL_CONSTANT_ALPHA", "GL_ONE_MINUS_CONSTANT_ALPHA" };

        const char* s_CurrentBlendingSourceFactor = s_BlendFuncs[6];
        const char* s_CurrentBlendingDestinationFactor = s_BlendFuncs[7];

        GLenum m_CurrentBlendingSourceFactor = GL_SRC_ALPHA;
        GLenum m_CurrentBlendingDestinationFactor = GL_ONE_MINUS_SRC_ALPHA;

        const char* s_BlendEquations[5] = { "GL_FUNC_ADD", "GL_FUNC_SUBTRACT", "GL_FUNC_REVERSE_SUBTRACT", "GL_MIN", "GL_MAX" };
        const char* s_CurrentBlendingEquation = s_BlendEquations[0];
        GLenum m_CurrentBlendEquation = GL_FUNC_ADD;

        bool m_GLBlendColorEnabled = false;
        glm::vec4 m_GLBlendColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        struct Light
        {
            glm::vec3 direction;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;

            Light() : direction(0.0f, 0.0f, 1.0f), ambient(0.0f), diffuse(0.0f), specular(0.0f) {}
        }m_DirectionalLight;

        struct Material
        {
            glm::vec3 color;
            float specularStrength;
            float shininess;

            Material() : color(1.0f), specularStrength(1.0f), shininess(32.0f) {}
        }m_ModelMaterial;

        float m_PlaneVertices[36] =
        {
            // Vertex Positions   // Texture Coordinates

            // Top Face
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        glm::vec3 m_WindowPositions[4] =
        {
            glm::vec3(0.2f, 0.4f, 0.0f),
            glm::vec3(0.0f, 0.4f, 0.4f),
            glm::vec3(0.0f, 0.45f, 0.8f),
            glm::vec3(0.2f, 0.5f, 1.2f)
        };

        std::map<float, glm::vec3> m_SortedWindowPositions;
    };
}