// Link-> https://www.learnopengl.com/Getting-started/Shaders

#include "../../../Lectures.h"
#include "Shader.h"

// Namespace of each lectures to avoid ambiguity.
namespace GettingStarted
{
    class ShaderLecture : public Lecture
    {
    public:
        ShaderLecture();
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible, int width, int height) override;
        virtual void CloseLecture() override;
    private:
        Shader m_Shader;
        uint32_t m_VAO, m_VBO;
        bool m_Animate = true;
        bool m_Invert = false;
        float m_Offset[2] = {0.0f, 0.0f};

        float m_TriangleVertices[18] = 
        {
            // Vertex Positions     // Vertex Colors
            -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     
             0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
             0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f
        };
    };
}