// Link -> https://www.learnopengl.com/Lighting/Colors

#include "../../GettingStarted/Shader/Shader.h"
#include "../../../Lectures.h"
#include "../../../../vendor/glad/include/glad.h"
#include "../../../../vendor/glm/glm.hpp"

namespace Lighting
{
    class ColorLecture : public Lecture
    {
    public:
        ColorLecture(int width, int height);
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible, int width, int height) override;
        virtual void CloseLecture() override;
    private:
        void Reset(int width, int height);
    private:
        GettingStarted::Shader m_LightShader, m_CubeShader;
        uint32_t m_VAO, m_VBO;
        glm::mat4 m_Model, m_View, m_Projection, m_CubeModel;
        uint32_t m_Width, m_Height;

        glm::vec3 m_LightColor, m_CubeColor;

        float m_CubeVertices[108] = 
        {
            // Vertex Positions

            // Front Face
            -0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
            -0.5f,  0.5f, -0.5f,  
            -0.5f, -0.5f, -0.5f,  

            // Back Face
            -0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f,  

            // Left Face
            -0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f, -0.5f,  
            -0.5f, -0.5f, -0.5f,  
            -0.5f, -0.5f, -0.5f,  
            -0.5f, -0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f,  

            // Right Face
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  

            // Bottom Face
            -0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f, -0.5f,  

            // Top Face
            -0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f, -0.5f
        };
    };
}
