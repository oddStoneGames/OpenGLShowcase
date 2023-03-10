#include "ModelLoadingLecture.h"
#include "../GettingStarted/Camera/Camera.h"
#include "../../../vendor/glm/gtc/matrix_transform.hpp"
#include <string>
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

namespace ModelLoading
{
    ModelLoadingLecture::ModelLoadingLecture(int width, int height) : 
        m_LightShader(PROJECT_DIR"/src/Lectures/ModelLoading/Shaders/LightSphere.vert",
                      PROJECT_DIR"/src/Lectures/ModelLoading/Shaders/LightSphere.frag"), 
        m_ModelShader(PROJECT_DIR"/src/Lectures/ModelLoading/Shaders/Model.vert",
                      PROJECT_DIR"/src/Lectures/ModelLoading/Shaders/Model.frag"),
        m_LightSphere(),
        m_Model()
    {
        m_LectureLink = "https://www.learnopengl.com/Lighting/Model-Loading/Assimp";

        // Initialize default values.
        Reset(width, height);
    }

    void ModelLoadingLecture::OpenLecture()
    {
        // Reset Options.
        Reset(m_Width, m_Height);

        // Load Models
        m_LightSphere.LoadModel(PROJECT_DIR"/src/Assets/Models/ModelLoading/Sphere.obj");
        m_Model.LoadModel(PROJECT_DIR"/src/Assets/Models/ModelLoading/Pistol.obj");

        // Set Uniforms.
        m_LightShader.Use();
        m_LightShader.SetMat4("u_Model", m_LightSphere_ModelMatrix);
        m_LightShader.SetMat4("u_View", m_ViewMatrix);
        m_LightShader.SetMat4("u_Projection", m_ProjectionMatrix);
        GettingStarted::Shader::Unbind();

        m_ModelShader.Use();
        m_ModelShader.SetMat4("u_Model", m_Model_ModelMatrix);
        m_ModelShader.SetMat4("u_View", m_ViewMatrix); 
        m_ModelShader.SetMat4("u_Projection", m_ProjectionMatrix);
        m_ModelShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_ModelShader.SetFloat3("u_PointLight.position",  m_PointLight.position);
        m_ModelShader.SetFloat3("u_PointLight.ambient",   m_PointLight.ambient);
        m_ModelShader.SetFloat3("u_PointLight.diffuse",   m_PointLight.diffuse);
        m_ModelShader.SetFloat3("u_PointLight.specular",  m_PointLight.specular);
        m_ModelShader.SetFloat ("u_PointLight.constant",  m_PointLight.constant);
        m_ModelShader.SetFloat ("u_PointLight.linear",    m_PointLight.linear);
        m_ModelShader.SetFloat ("u_PointLight.quadratic", m_PointLight.quadratic);

        m_ModelShader.SetFloat3("u_Material.color", m_ModelMaterial.color);
        m_ModelShader.SetFloat("u_Material.specularStrength", m_ModelMaterial.specularStrength);
        m_ModelShader.SetFloat("u_Material.shininess", m_ModelMaterial.shininess);

        GettingStarted::Shader::Unbind();

        // Enable Depth Testing.
        glEnable(GL_DEPTH_TEST);
    }

    void ModelLoadingLecture::RenderLecture(bool settingsVisible, int width, int height)
    {
        if (settingsVisible)
        {
            // Draw ImGui Settings Menu.
            ImGui::Begin("Light Settings");

            ImGui::NewLine();

            if (!m_RotateLightSphere)
                ImGui::DragFloat3(LabelPrefix("Position: ").c_str(), &m_PointLight.position[0], 0.5f, -1000.0f, 1000.0f, "%.2f");
            else
                ImGui::DragFloat(LabelPrefix("Position Y: ").c_str(), &m_PointLight.position.y, 0.5f, -1000.0f, 1000.0f, "%.2f");
            
            ImGui::Checkbox(LabelPrefix("Rotate Light Sphere: ").c_str(), &m_RotateLightSphere);
            if(m_RotateLightSphere)
                ImGui::DragFloat(LabelPrefix("Rotation Radius: ").c_str(), &m_LightSphereRotationRadius, 0.5f, 0.0f, 1000.0f, "%.2f");
            ImGui::ColorEdit3(LabelPrefix("Ambient Color: ").c_str(), &m_PointLight.ambient[0]);
            ImGui::ColorEdit3(LabelPrefix("Diffuse Color: ").c_str(), &m_PointLight.diffuse[0]);
            ImGui::ColorEdit3(LabelPrefix("Specular Color: ").c_str(), &m_PointLight.specular[0]);
            ImGui::DragFloat(LabelPrefix("Constant: ").c_str(), &m_PointLight.constant, 0.05f, 0.001f, 10.0f, "%.2f");
            ImGui::DragFloat(LabelPrefix("Linear: ").c_str(), &m_PointLight.linear, 0.5f, 0.0f, 1000.0f, "%.2f");
            ImGui::DragFloat(LabelPrefix("Quadratic: ").c_str(), &m_PointLight.quadratic, 0.5f, 0.0f, 1000.0f, "%.2f");
            ImGui::NewLine();

            if (DrawButtonCentered("Reset")) Reset(width, height, 1);

            ImGui::End();

            ImGui::Begin("Model Settings");
            ImGui::NewLine();
            
            if (DrawButtonCentered("Choose Custom Model"))
                ChooseCustomModel();

            ImGui::NewLine();
            ImGui::TextWrapped("We make an assumption that the textures are in the same directory where the model is, if not then textures will not be loaded.");
            
            ImGui::NewLine();
            ImGui::DragFloat3(LabelPrefix("Position: ").c_str(), &m_ModelPosition[0], 0.5f, -1000.0f, 1000.0f, "%.1f");
            ImGui::DragFloat3(LabelPrefix("Rotation: ").c_str(), &m_ModelRotation[0], 0.5f, -360.0f, 360.0f, "%.1f");
            ImGui::DragFloat3(LabelPrefix("Scale: ").c_str(), &m_ModelScale[0], 0.01f, -5.0f, 5.0f, "%.2f");
            ImGui::NewLine();

            DrawTextCentered("Material Settings");

            ImGui::NewLine();
            ImGui::ColorEdit3(LabelPrefix("Color: ").c_str(), &m_ModelMaterial.color[0]);
            ImGui::DragFloat(LabelPrefix("Specular Strength: ").c_str(), &m_ModelMaterial.specularStrength, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat(LabelPrefix("Shininess: ").c_str(), &m_ModelMaterial.shininess, 0.05f, 0.05f, 1000.0f, "%.2f");
            ImGui::NewLine();

            if (DrawButtonCentered("Reset")) Reset(width, height, 2);

            ImGui::End();
            
        }

        GettingStarted::Camera* cam = GettingStarted::Camera::GetInstance();

        // Reset Matrices.
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionMatrix = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;
        float aspect = (float)m_Width/(float)m_Height;

        m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        m_ViewMatrix = cam->GetViewMatrix();
        
        // Draw Light Sphere
        m_LightShader.Use();

        // Update Shader Uniforms.
        m_LightShader.SetMat4("u_View", m_ViewMatrix);
        m_LightShader.SetMat4("u_Projection", m_ProjectionMatrix);
        m_LightShader.SetFloat3("u_LightColor", m_PointLight.diffuse);

        if (m_RotateLightSphere)
        {
            m_PointLight.position.x = sin(glfwGetTime()) * m_LightSphereRotationRadius;
            m_PointLight.position.z = cos(glfwGetTime()) * m_LightSphereRotationRadius;
        }

        m_LightSphere_ModelMatrix = glm::mat4(1.0f);
        m_LightSphere_ModelMatrix = glm::translate(m_LightSphere_ModelMatrix, m_PointLight.position);
        m_LightSphere_ModelMatrix = glm::scale(m_LightSphere_ModelMatrix, glm::vec3(0.35f));

        m_LightShader.SetMat4("u_Model", m_LightSphere_ModelMatrix);

        m_LightSphere.Draw(m_LightShader);


        // Draw Model
        m_ModelShader.Use();

        // Update Shader Uniforms.
        m_ModelShader.SetMat4("u_View", m_ViewMatrix);
        m_ModelShader.SetMat4("u_Projection", m_ProjectionMatrix);
        m_ModelShader.SetFloat3("u_ViewPos", GettingStarted::Camera::GetInstance()->Position);

        m_Model_ModelMatrix = glm::mat4(1.0f);
        m_Model_ModelMatrix = glm::translate(m_Model_ModelMatrix, m_ModelPosition);
        m_Model_ModelMatrix = glm::rotate(m_Model_ModelMatrix, glm::radians(m_ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Model_ModelMatrix = glm::rotate(m_Model_ModelMatrix, glm::radians(m_ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_Model_ModelMatrix = glm::rotate(m_Model_ModelMatrix, glm::radians(m_ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_Model_ModelMatrix = glm::scale(m_Model_ModelMatrix, m_ModelScale);

        m_ModelShader.SetMat4("u_Model", m_Model_ModelMatrix);
        m_ModelShader.SetFloat3("u_PointLight.position", m_PointLight.position);
        m_ModelShader.SetFloat3("u_PointLight.ambient", m_PointLight.ambient);
        m_ModelShader.SetFloat3("u_PointLight.diffuse", m_PointLight.diffuse);
        m_ModelShader.SetFloat3("u_PointLight.specular", m_PointLight.specular);
        m_ModelShader.SetFloat("u_PointLight.constant", m_PointLight.constant);
        m_ModelShader.SetFloat("u_PointLight.linear", m_PointLight.linear);
        m_ModelShader.SetFloat("u_PointLight.quadratic", m_PointLight.quadratic);

        m_ModelShader.SetFloat3("u_Material.color", m_ModelMaterial.color);
        m_ModelShader.SetFloat("u_Material.specularStrength", m_ModelMaterial.specularStrength);
        m_ModelShader.SetFloat("u_Material.shininess", m_ModelMaterial.shininess);

        m_Model.Draw(m_ModelShader);
    }

    void ModelLoadingLecture::CloseLecture()
    {
        // Unload all the models to free memory.
        m_LightSphere.Free();
        m_Model.Free();

        // Reset Depth Testing to false.
        glDisable(GL_DEPTH_TEST);
    }

    void ModelLoadingLecture::ChooseCustomModel()
    {
        // Open a Popup to Select File.
        char filename[FILENAME_MAX] = "";

#ifdef WIN32
        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = NULL;
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "";

        std::string fileNameStr;
        if (GetOpenFileName(&ofn))
        {
            fileNameStr = filename;
            // Load Model.
            m_Model.LoadModel(fileNameStr);
        }
#else
        // Linux File open dialog
        FILE* f = popen("zenity --file-selection", "r");
        fgets(filename, FILENAME_MAX, f);
        std::string filePath = filename;
        // Replace the Null Char with Espace Backslashes otherwise stb_load won't be able to open the file.
        filePath.replace(filePath.length() - 1, 1, '\0', '\\');
        // Load Model.
        m_Model.LoadModel(filePath);
#endif
    }

    void ModelLoadingLecture::Reset(int width, int height, int resetState)
    {
        // Default Matrices
        m_ViewMatrix = glm::mat4(1.0f);
        m_ProjectionMatrix = glm::mat4(1.0f);
        m_Width = width;
        m_Height = height;
        m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, 3.0f));
        m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f);

        if (resetState == 1 || resetState > 2)
        {
            // Reset Light Values.
            m_LightSphere_ModelMatrix = glm::mat4(1.0f);
            m_RotateLightSphere = true;
            m_LightSphereRotationRadius = 8.0f;

            m_PointLight.position = glm::vec3(0.0f, 0.0f, -m_LightSphereRotationRadius);
            m_PointLight.ambient = glm::vec3(0.23f);
            m_PointLight.diffuse = glm::vec3(0.5f);
            m_PointLight.specular = glm::vec3(1.0f);
            m_PointLight.constant = 1.0f;
            m_PointLight.linear = 0.027f;
            m_PointLight.quadratic = 0.0019f;

            m_LightSphere_ModelMatrix = glm::translate(m_LightSphere_ModelMatrix, m_PointLight.position);
            m_LightSphere_ModelMatrix = glm::scale(m_LightSphere_ModelMatrix, glm::vec3(0.35f));
        }
        
        if (resetState >= 2)
        {
            if(resetState == 2)
                m_Model.LoadModel(PROJECT_DIR"/src/Assets/Models/ModelLoading/Pistol.obj");

            m_ModelMaterial.color = glm::vec3(1.0f);
            m_ModelMaterial.specularStrength = 1.0f;
            m_ModelMaterial.shininess = 32.0f;

            m_ModelPosition = glm::vec3(0.0f);
            m_ModelRotation = glm::vec3(0.0f);
            m_ModelScale = glm::vec3(1.0f);

            m_Model_ModelMatrix = glm::mat4(1.0f);
            m_Model_ModelMatrix = glm::translate(m_Model_ModelMatrix, m_ModelPosition);
            m_Model_ModelMatrix = glm::rotate(m_Model_ModelMatrix, glm::radians(m_ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m_Model_ModelMatrix = glm::rotate(m_Model_ModelMatrix, glm::radians(m_ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_Model_ModelMatrix = glm::rotate(m_Model_ModelMatrix, glm::radians(m_ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            m_Model_ModelMatrix = glm::scale(m_Model_ModelMatrix, m_ModelScale);
        }
    }
}
