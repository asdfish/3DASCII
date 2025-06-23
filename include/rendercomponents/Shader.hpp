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

        Shader(Shader&& other) noexcept
            : m_id(other.m_id), m_filePath(std::move(other.m_filePath)), m_name(std::move(other.m_name)), m_type(other.m_type)
        {
            other.m_id = 0; // prevent deletion from moved-from object
        }

        // Move assignment
        Shader& operator=(Shader&& other) noexcept
        {
            if (this != &other)
            {
                glDeleteShader(m_id); // clean current
                m_id = other.m_id;
                m_filePath = std::move(other.m_filePath);
                m_name = std::move(other.m_name);
                m_type = other.m_type;
                other.m_id = 0;
            }
            return *this;
        }

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        inline GLuint GetID() const { return m_id;}
        inline std::string GetName() const { return m_name;}
        inline GLenum GetType() const { return m_type;}
        inline std::string GetFilePath() const { return m_filePath;}


        void SetUniform4f(const std::string& name, const glm::vec4 val);

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