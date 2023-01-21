// Hello Triangle Lecture!
#include "HelloTriangle.h"
#include <iostream>

namespace GettingStarted
{
    /// @brief Intialize Lecture Link & all the other variables necessary.
    HelloTriangle::HelloTriangle()
    {
        m_LectureLink = "xdg-open https://www.learnopengl.com/Getting-started/Hello-Triangle";

        // Store Initial Values.
        for(int i = 0; i < 12; i++)
        {
            m_InitialQuadVertices[i] = m_QuadVertices[i];
            if(i < 9) m_InitialTriangleVertices[i] = m_TriangleVertices[i];
        }
    }

    void HelloTriangle::OpenLecture()
    {
        // Create VAO, VBO & Shaders.

        // Vertex Shader
        uint32_t vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderID, 1, &m_VertexShaderCode, NULL);
        glCompileShader(vertexShaderID);
        // Check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
        }

        // Fragment Shader
        uint32_t fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderID, 1, &m_FragmentShaderCode, NULL);
        glCompileShader(fragmentShaderID);
        // Check for shader compile errors
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
        }

        // Link shaders
        m_ShaderProgramID = glCreateProgram();
        glAttachShader(m_ShaderProgramID, vertexShaderID);
        glAttachShader(m_ShaderProgramID, fragmentShaderID);
        glLinkProgram(m_ShaderProgramID);
        // check for linking errors
        glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
        if (!success) 
        {
            glGetProgramInfoLog(m_ShaderProgramID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
        }

        // Delete Shaders after Shader Program Creation.
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        // Generate VAO, VBO & EBO & setup vertex attribute pointers to pass the data to GPU correctly.
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        PopulateBuffers();
    }

    void HelloTriangle::RenderLecture(bool settingsVisible)
    {
        if(settingsVisible)
        {
            // Render ImGui Settings Window!
            ImGui::Begin("Hello Triangle");
            
            if(ImGui::Checkbox(LabelPrefix("Show Wireframe: ").c_str(), &m_Wireframe))
                // Switch Wireframe Rendering!
                glPolygonMode(GL_FRONT_AND_BACK, m_Wireframe ? GL_LINE : GL_FILL);

            if(ImGui::Checkbox(LabelPrefix("Indexed Drawing: ").c_str(), &m_IndexedDraw))
                PopulateBuffers();

            ImGui::NewLine();
            if(m_IndexedDraw)
            {
                // Quad Options.

                //TODO: Add Big Style Font
                ImGui::SetWindowFontScale(1.3f);
                DrawTextCentered("Vertices");
                ImGui::SetWindowFontScale(1.0f);
                ImGui::Spacing();

                const char* labels[4] = {"Bottom Left", "Bottom Right", "Top Right", "Top Left"};
                for(int i = 0; i < 4; i++)
                {
                    float verts[3];
                    std::copy(m_QuadVertices + (i*3), m_QuadVertices + (i*3+3), verts);
                    ImGui::DragFloat3(LabelPrefix(labels[i]).c_str(), verts, 0.01f, -1.0f, 1.0f, "%.2f");
                    m_QuadVertices[i*3] = verts[0];
                    m_QuadVertices[i*3 + 1] = verts[1];
                    m_QuadVertices[i*3 + 2] = verts[2];
                }
            }
            else
            {
                // Triangle Options.

                //TODO: Add Big Style Font
                ImGui::SetWindowFontScale(1.3f);
                DrawTextCentered("Vertices");
                ImGui::SetWindowFontScale(1.0f);
                ImGui::Spacing();

                const char* labels[3] = {"Bottom Left", "Bottom Right", "Top Middle"};
                for(int i = 0; i < 3; i++)
                {
                    float verts[3];
                    std::copy(m_TriangleVertices + (i*3), m_TriangleVertices + (i*3+3), verts);
                    ImGui::DragFloat3(LabelPrefix(labels[i]).c_str(), verts, 0.01f, -1.0f, 1.0f, "%.2f");
                    m_TriangleVertices[i*3] = verts[0];
                    m_TriangleVertices[i*3 + 1] = verts[1];
                    m_TriangleVertices[i*3 + 2] = verts[2];
                }
            }

            ImGui::NewLine();
            if(DrawButtonCentered("Reset"))
            {
                // Reset Vertices.
                for(int i = 0; i < 12; i++)
                {
                    m_QuadVertices[i] = m_InitialQuadVertices[i];
                    if(i < 9) m_TriangleVertices[i] = m_InitialTriangleVertices[i];
                }
            }
            ImGui::End();
        }
        
        PopulateBuffers();

        // Bind VAO & Use Shader Program.
        glUseProgram(m_ShaderProgramID);
        glBindVertexArray(m_VAO);

        // Render Triangle/Quad
        if(m_IndexedDraw)
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, 3);

        // Unbind
        glBindVertexArray(0);
        glUseProgram(0);
    }

    /// @brief Free the resources loaded.
    void HelloTriangle::CloseLecture()
    {
        // Reset Polygon Drawmode.
        m_Wireframe = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteProgram(m_ShaderProgramID);
    }

    /// @brief Passes Data to VBO & EBO.
    void HelloTriangle::PopulateBuffers()
    {
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        
        if(m_IndexedDraw)
        {
            // Pass Quad Data.
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_QuadVertices), m_QuadVertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_STATIC_DRAW);
        }else
        {
            // Pass Triangle Data.
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_TriangleVertices), m_TriangleVertices, GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 
    }
}