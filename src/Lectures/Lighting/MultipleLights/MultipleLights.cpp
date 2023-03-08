#include "MultipleLights.h"
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
    MultipleLights::MultipleLights(int width, int height) : 
        m_LightShader(PROJECT_DIR"/src/Lectures/Lighting/MultipleLights/LightCube.vert",
                      PROJECT_DIR"/src/Lectures/Lighting/MultipleLights/LightCube.frag"), 
        m_CubeShader(PROJECT_DIR"/src/Lectures/Lighting/MultipleLights/MultipleLights.vert",
                     PROJECT_DIR"/src/Lectures/Lighting/MultipleLights/MultipleLights.frag")
    {
        m_LectureLink = "https://www.learnopengl.com/Lighting/Multiple-lights";

        // Initialize default values.
        Reset(width, height);
    }

    void MultipleLights::OpenLecture()
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
        GettingStarted::Shader::Unbind();

        m_CubeShader.Use();
        m_CubeShader.SetMat4("u_Model", m_CubeModel);
        m_CubeShader.SetMat4("u_View", m_View); 
        m_CubeShader.SetMat4("u_Projection", m_Projection);
        m_CubeShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_CubeShader.SetFloat3("u_DirectionalLight.direction", m_DirectionalLight.direction);
        m_CubeShader.SetFloat3("u_DirectionalLight.ambient",   m_DirectionalLight.ambient);
        m_CubeShader.SetFloat3("u_DirectionalLight.diffuse",   m_DirectionalLight.diffuse);
        m_CubeShader.SetFloat3("u_DirectionalLight.specular",  m_DirectionalLight.specular);

        m_CubeShader.SetFloat3("u_PointLights[0].position",  m_PointLights[0].position);
        m_CubeShader.SetFloat3("u_PointLights[0].ambient",   m_PointLights[0].ambient);
        m_CubeShader.SetFloat3("u_PointLights[0].diffuse",   m_PointLights[0].diffuse);
        m_CubeShader.SetFloat3("u_PointLights[0].specular",  m_PointLights[0].specular);
        m_CubeShader.SetFloat ("u_PointLights[0].constant",  m_PointLights[0].constant);
        m_CubeShader.SetFloat ("u_PointLights[0].linear",    m_PointLights[0].linear);
        m_CubeShader.SetFloat ("u_PointLights[0].quadratic", m_PointLights[0].quadratic);

        m_CubeShader.SetFloat3("u_PointLights[1].position",  m_PointLights[1].position);
        m_CubeShader.SetFloat3("u_PointLights[1].ambient",   m_PointLights[1].ambient);
        m_CubeShader.SetFloat3("u_PointLights[1].diffuse",   m_PointLights[1].diffuse);
        m_CubeShader.SetFloat3("u_PointLights[1].specular",  m_PointLights[1].specular);
        m_CubeShader.SetFloat ("u_PointLights[1].constant",  m_PointLights[1].constant);
        m_CubeShader.SetFloat ("u_PointLights[1].linear",    m_PointLights[1].linear);
        m_CubeShader.SetFloat ("u_PointLights[1].quadratic", m_PointLights[1].quadratic);

        m_CubeShader.SetFloat3("u_SpotLight.position",    m_SpotLight.position);
        m_CubeShader.SetFloat3("u_SpotLight.direction",   m_SpotLight.direction);
        m_CubeShader.SetFloat3("u_SpotLight.ambient",     m_SpotLight.ambient);
        m_CubeShader.SetFloat3("u_SpotLight.diffuse",     m_SpotLight.diffuse);
        m_CubeShader.SetFloat3("u_SpotLight.specular",    m_SpotLight.specular);
        m_CubeShader.SetFloat ("u_SpotLight.constant",    m_SpotLight.constant);
        m_CubeShader.SetFloat ("u_SpotLight.linear",      m_SpotLight.linear);
        m_CubeShader.SetFloat ("u_SpotLight.quadratic",   m_SpotLight.quadratic);
        m_CubeShader.SetFloat ("u_SpotLight.cutOff",      glm::cos(glm::radians(m_SpotLight.cutOff)));
        m_CubeShader.SetFloat ("u_SpotLight.outerCutOff", glm::cos(glm::radians(m_SpotLight.outerCutOff)));

        // Set Default Light Type to Directional Light
        m_CubeShader.SetInt("u_LightType", 0);

        m_CubeShader.SetInt("u_Material.diffuse",  0);
        m_CubeShader.SetInt("u_Material.specular", 1);
        m_CubeShader.SetInt("u_Material.emission", 2);
        m_CubeShader.SetFloat("u_Material.shininess", 32.0f);

        GettingStarted::Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void MultipleLights::RenderLecture(bool settingsVisible, int width, int height)
    {
        GettingStarted::Camera* cam = GettingStarted::Camera::GetInstance();

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
        m_LightShader.SetMat4("u_View", m_View);
        m_LightShader.SetMat4("u_Projection", m_Projection);

        // Bind Light VAO.
        glBindVertexArray(m_VAO);

        for (int i = 0; i < 3; i++)
        {
            m_Model = glm::mat4(1.0f);
            m_Model = glm::translate(m_Model, i <= 1 ? m_PointLights[i].position : m_SpotLight.position);
            m_Model = glm::scale(m_Model, glm::vec3(0.25f));
            m_LightShader.SetMat4("u_Model", m_Model);
            m_LightShader.SetFloat3("u_LightColor", i <= 1 ? m_PointLights[i].diffuse : m_SpotLight.diffuse);

            // Draw Indexed.
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
        m_CubeShader.SetMat4("u_View", m_View);
        m_CubeShader.SetMat4("u_Projection", m_Projection);
        m_CubeShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_CubeShader.SetFloat3("u_DirectionalLight.direction", m_DirectionalLight.direction);
        m_CubeShader.SetFloat3("u_DirectionalLight.ambient", m_DirectionalLight.ambient);
        m_CubeShader.SetFloat3("u_DirectionalLight.diffuse", m_DirectionalLight.diffuse);
        m_CubeShader.SetFloat3("u_DirectionalLight.specular", m_DirectionalLight.specular);

        m_CubeShader.SetFloat3("u_PointLights[0].position", m_PointLights[0].position);
        m_CubeShader.SetFloat3("u_PointLights[0].ambient", m_PointLights[0].ambient);
        m_CubeShader.SetFloat3("u_PointLights[0].diffuse", m_PointLights[0].diffuse);
        m_CubeShader.SetFloat3("u_PointLights[0].specular", m_PointLights[0].specular);
        m_CubeShader.SetFloat("u_PointLights[0].constant", m_PointLights[0].constant);
        m_CubeShader.SetFloat("u_PointLights[0].linear", m_PointLights[0].linear);
        m_CubeShader.SetFloat("u_PointLights[0].quadratic", m_PointLights[0].quadratic);

        m_CubeShader.SetFloat3("u_PointLights[1].position", m_PointLights[1].position);
        m_CubeShader.SetFloat3("u_PointLights[1].ambient", m_PointLights[1].ambient);
        m_CubeShader.SetFloat3("u_PointLights[1].diffuse", m_PointLights[1].diffuse);
        m_CubeShader.SetFloat3("u_PointLights[1].specular", m_PointLights[1].specular);
        m_CubeShader.SetFloat("u_PointLights[1].constant", m_PointLights[1].constant);
        m_CubeShader.SetFloat("u_PointLights[1].linear", m_PointLights[1].linear);
        m_CubeShader.SetFloat("u_PointLights[1].quadratic", m_PointLights[1].quadratic);

        m_CubeShader.SetFloat3("u_SpotLight.position", m_SpotLight.position);
        m_CubeShader.SetFloat3("u_SpotLight.direction", m_SpotLight.direction);
        m_CubeShader.SetFloat3("u_SpotLight.ambient", m_SpotLight.ambient);
        m_CubeShader.SetFloat3("u_SpotLight.diffuse", m_SpotLight.diffuse);
        m_CubeShader.SetFloat3("u_SpotLight.specular", m_SpotLight.specular);
        m_CubeShader.SetFloat("u_SpotLight.constant", m_SpotLight.constant);
        m_CubeShader.SetFloat("u_SpotLight.linear", m_SpotLight.linear);
        m_CubeShader.SetFloat("u_SpotLight.quadratic", m_SpotLight.quadratic);
        m_CubeShader.SetFloat("u_SpotLight.cutOff", glm::cos(glm::radians(m_SpotLight.cutOff)));
        m_CubeShader.SetFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(m_SpotLight.outerCutOff)));

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            m_CubeModel = glm::mat4(1.0f);
            m_CubeModel = glm::translate(m_CubeModel, m_CubePositions[i]);
            float angle = 20.0f * i;
            m_CubeModel = glm::rotate(m_CubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_CubeShader.SetMat4("u_Model", m_CubeModel);

            // Draw Indexed.
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        GettingStarted::Shader::Unbind();
    }

    void MultipleLights::CloseLecture()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteVertexArrays(1, &m_CubeVAO);
        glDeleteBuffers(1, &m_VBO);

        // Reset Depth Testing to false.
        glDisable(GL_DEPTH_TEST);
    }

    void MultipleLights::LoadTexture(const char* texturePath, uint32_t textureID)
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

    void MultipleLights::Reset(int width, int height)
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

        // Reset Light Values.
        m_DirectionalLight.direction = glm::vec3(-0.3f, 0.05f, -0.5f);
        m_DirectionalLight.ambient = glm::vec3(0.1f);
        m_DirectionalLight.diffuse = glm::vec3(0.5f);
        m_DirectionalLight.specular = glm::vec3(1.0f);

        m_PointLights[0].position = m_LightCubePositions[0];
        m_PointLights[0].ambient = glm::vec3(0.1f);
        m_PointLights[0].diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
        m_PointLights[0].specular = glm::vec3(1.0f);
        m_PointLights[0].constant = 1.0f;
        m_PointLights[0].linear = 0.09f;
        m_PointLights[0].quadratic = 0.032f;

        m_PointLights[1].position = m_LightCubePositions[1];
        m_PointLights[1].ambient = glm::vec3(0.1f);
        m_PointLights[1].diffuse = glm::vec3(0.0f, 0.5f, 0.0f);
        m_PointLights[1].specular = glm::vec3(1.0f);
        m_PointLights[1].constant = 1.0f;
        m_PointLights[1].linear = 0.09f;
        m_PointLights[1].quadratic = 0.032f;

        m_SpotLight.position = m_LightCubePositions[2];
        m_SpotLight.direction = glm::vec3(-0.3f, 0.05f, -0.5f);
        m_SpotLight.ambient = glm::vec3(0.1f);
        m_SpotLight.diffuse = glm::vec3(0.5f, 0.0f, 0.5f);
        m_SpotLight.specular = glm::vec3(1.0f);
        m_SpotLight.constant = 1.0f;
        m_SpotLight.linear = 0.09f;
        m_SpotLight.quadratic = 0.032f;
        m_SpotLight.cutOff = 25.0f;
        m_SpotLight.outerCutOff = 35.0f;

        m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, 3.0f));
        m_Model = glm::scale(m_Model, glm::vec3(0.5f));
        m_CubeModel = glm::translate(m_CubeModel, glm::vec3(0.7f, -0.3f, -1.1f));
        m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/(float)m_Height, 0.1f, 100.0f);
    }
}