// Link -> https://www.learnopengl.com/Lighting/Model-Loading/Assimp

#include "../GettingStarted/Shader/Shader.h"
#include "../../Lectures.h"
#include "../../../vendor/glad/include/glad.h"
#include "../../../vendor/glm/glm.hpp"
#include "Model.h"

namespace ModelLoading
{
    class ModelLoadingLecture : public Lecture
    {
    public:
        ModelLoadingLecture(int width, int height);
        virtual void OpenLecture() override;
        virtual void RenderLecture(bool settingsVisible, int width, int height) override;
        virtual void CloseLecture() override;
    private:
        void ChooseCustomModel();
        void Reset(int width, int height, int resetState = 3);
    private:
        GettingStarted::Shader m_LightShader, m_ModelShader;
        Model m_LightSphere, m_Model;

        float m_LightSphereRotationRadius;
        bool m_RotateLightSphere;

        glm::vec3 m_ModelPosition, m_ModelRotation, m_ModelScale;
        glm::mat4 m_LightSphere_ModelMatrix, m_Model_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix;
        uint32_t m_Width, m_Height;

        struct Light
        {
            glm::vec3 position;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float constant;
            float linear;
            float quadratic;

            Light() : position(0.0f), ambient(0.0f), diffuse(0.0f), specular(0.0f),
                constant(1.0f), linear(0.0f), quadratic(0.0f) {}
        }m_PointLight;

        struct Material
        {
            glm::vec3 color;
            float specularStrength;
            float shininess;

            Material() : color(1.0f), specularStrength(1.0f), shininess(32.0f) {}
        }m_ModelMaterial;
    };
}
