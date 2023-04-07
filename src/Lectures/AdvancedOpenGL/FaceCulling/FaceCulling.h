// Link -> https://www.learnopengl.com/Advanced-OpenGL/Face-culling

#include "../../GettingStarted/Shader/Shader.h"
#include "../../../Lectures.h"
#include "../../../../vendor/glad/include/glad.h"
#include "../../../../vendor/glm/glm.hpp"

namespace AdvancedOpenGL
{
    class FaceCulling : public Lecture
    {
    public:
        FaceCulling(int width, int height);
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible, int width, int height) override;
        virtual void CloseLecture() override;
    private:
        void UpdateCullingAttributes();
        void LoadTexture(const char* texturePath, uint32_t textureID);
        void Reset(int width, int height);
    private:
        GettingStarted::Shader m_Shader;
        uint32_t m_CubeVAO, m_PlaneVAO;
        uint32_t m_CubeVBO, m_PlaneVBO;
        glm::mat4 m_Model, m_View, m_Projection;
        uint32_t m_Width, m_Height;

        struct Light
        {
            glm::vec3 direction;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;

            Light() : direction(0.0f), ambient(0.0f), diffuse(0.0f), specular(0.0f) {}
        }m_DirectionalLight;

        uint32_t m_DiffuseTextureID, m_SpecularTextureID;
        uint32_t m_PlaneDiffuseTextureID;

        bool m_Initialized = false;

        bool m_FaceCullingEnabled = true;
        const char* s_CullFace[3] = { "GL_BACK", "GL_FRONT", "GL_FRONT_AND_BACK" };
        const char* s_CurrentCullFace = "GL_BACK";
        GLenum m_CurrentCullFace = GL_BACK;
        const char* s_FrontFace[2] = { "GL_CCW", "GL_CW" };
        const char* s_CurrentFrontFace = "GL_CCW";
        GLenum m_CurrentFrontFace = GL_CCW;

        float m_CubeVertices[288] = 
        {
            // Vertex Positions   //     Normals       // Texture Coordinates

            // Back Face
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

            // Front Face         
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            // Left Face
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            // Right Face
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

            // Bottom Face
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            // Top Face
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
        };

        float m_PlaneVertices[48] =
        {
            // Vertex Positions   //     Normals       // Texture Coordinates
            
            // Top Face
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        // Position of Plane
        glm::vec3 m_PlanePosition = { 0.0f, -0.55f, 0.0f };

        // Positions of Cubes
        glm::vec3 m_CubePositions[2] = 
        {
            glm::vec3(1.0f, 0.0f, 2.0f),
            glm::vec3(4.0f, 0.0f, 1.0f)
        };
    };
}
