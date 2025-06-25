#pragma once
#include <string>
#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

struct ShaderData
{
    GLenum type;
    std::string name;
    std::string src;
};

class Shader
{
    public: 
        Shader(const std::string& filepath);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        inline GLuint GetID() const { return m_id;}
        inline std::string GetName() const { return m_name;}
        inline GLenum GetType() const { return m_type;}
        inline std::string GetFilePath() const { return m_filePath;}

    private:
        GLuint GetUniformLocation(const std::string& name);
        GLuint CompileShader(std::string src);
        std::string ParseShader(const std::string& filepath);

    private:
        GLuint m_id;
        GLenum m_type;
        std::string m_name;
        std::string m_filePath;

};