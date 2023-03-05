#ifndef SHADER_H
#define SHADER_H

// Include glad to get all the required OpenGL headers
#include "../../../../vendor/glad/include/glad.h"
#include "../../../../vendor/glm/glm.hpp"
#include <limits.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace GettingStarted
{
    class Shader
    {
    public:
        /// @brief Shader Program ID.
        unsigned int ID;
    public:
        /// @brief Constructor reads and builds the shader.
        /// @param vertexPath Vertex Shader File
        /// @param fragmentPath Fragment Shader File
        Shader(const char* vertexPath, const char* fragmentPath)
        {
            // 1. retrieve the vertex/fragment source code from filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            try 
            {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);

                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                // convert stream into string
                vertexCode   = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure& e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();
            // 2. compile shaders
            unsigned int vertex, fragment;
            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            CheckCompileErrors(vertex, "VERTEX");
            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            CheckCompileErrors(fragment, "FRAGMENT");
            // shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            CheckCompileErrors(ID, "PROGRAM");
            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
        
        ~Shader()
        {
            // Delete this Shader Program.
            glDeleteProgram(ID);
        }

        /// @brief Use/activate the shader.
        void Use() 
        { 
            glUseProgram(ID); 
        }

        // Utility uniform functions
        void SetBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void SetInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void SetFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void SetFloat2(const std::string &name, float value1, float value2) const
        {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
        }
        void SetFloat2(const std::string& name, glm::vec2 vec) const
        {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
        }
        void SetFloat3(const std::string &name, float value1, float value2, float value3) const
        {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
        }
        void SetFloat3(const std::string& name, glm::vec3 vec) const
        {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
        }
        void SetFloat4(const std::string &name, float value1, float value2, float value3, float value4) const
        {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
        }
        void SetFloat4(const std::string& name, glm::vec4 vec) const
        {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
        }
        void SetMat2(const std::string &name, const glm::mat2 &mat2) const
        {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat2[0][0]);
        }
        void SetMat3(const std::string &name, const glm::mat3 &mat3) const
        {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat3[0][0]);
        }
        void SetMat4(const std::string &name, glm::mat4 &mat4) const
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat4[0][0]);
        }
        /// @brief Unbinds any shader currently bound to OpenGL.
        static void Unbind()
        { 
            glUseProgram(0);
        }

    private:
        /// @brief Utility function for checking shader compilation/linking errors.
        /// @param shader Shader ID
        /// @param type Shader Type: VERTEX/FRAGMENT/PROGRAM
        void CheckCompileErrors(unsigned int shader, std::string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
                }
            }
        }
    };
}
#endif // !SHADER_H