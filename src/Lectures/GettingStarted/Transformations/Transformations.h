// Link -> https://www.learnopengl.com/Getting-started/Transformations

#include "../../../Lectures.h"
#include "../Shader/Shader.h"
#include "../../../../vendor/glad/include/glad.h"
#include "../../../../vendor/glm/glm.hpp"

namespace GettingStarted
{
    class Transformations : public Lecture
    {
    public:
        Transformations();
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible) override;
        virtual void CloseLecture() override;
    private:
        void LoadTexture(const char* texturePath);
        void Reset();
        void ApplyOperation(const char* operation);
    private:
        Shader m_Shader;
        uint32_t m_TextureID;
        uint32_t m_VAO, m_VBO, m_EBO;
        glm::mat4 m_Transform;
        glm::vec3 m_Position;
        /// @brief Each values represents the angle in degrees of rotation in x, y & z axis respectively.
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;
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

        const char* s_Operations[3] = {"Scale", "Rotate", "Translate"};
    };
}
