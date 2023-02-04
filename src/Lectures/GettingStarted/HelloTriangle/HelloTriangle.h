// Link -> https://www.learnopengl.com/Getting-started/Hello-Triangle

#include "../../../Lectures.h"

namespace GettingStarted
{
    class HelloTriangle : public Lecture
    {
    public:
        HelloTriangle();
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible, int width, int height) override;
        virtual void CloseLecture() override;
    private:
        void PopulateBuffers();
        uint32_t m_ShaderProgramID;
        uint32_t m_VAO, m_VBO, m_EBO;
        
        const char* m_VertexShaderCode = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

        const char* m_FragmentShaderCode = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n\0";

        /// @brief If true, then draws everything in wireframe mode.
        bool m_Wireframe = false;
        /// @brief If true, then Quad using EBO will be rendered, else Triangle using glDrawArrays will be rendered.
        bool m_IndexedDraw = false;
        
        // For Reseting.
        float m_InitialTriangleVertices[9];
        float m_InitialQuadVertices[12];
        
        float m_TriangleVertices[9] = 
        {
            -0.5f, -0.5f, 0.0f,     // Bottom Left
            0.5f, -0.5f, 0.0f,     // Bottom Right
            0.0f,  0.5f, 0.0f      // Top Middle
        };
        float m_QuadVertices[12] = 
        {
            -0.5f, -0.5f, 0.0f,     // Bottom Left
            0.5f, -0.5f, 0.0f,     // Bottom Right
            0.5f,  0.5f, 0.0f,     // Top Right
            -0.5f,  0.5f, 0.0f      // Top Left
        };
        unsigned int m_Indices[6] =
        {
            0, 1, 2,    // First Triangle
            2, 3, 0     // Second Triangle
        };
    };
}