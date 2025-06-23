#pragma once

#include <string>
#include <glad/gl.h>
#include <unordered_map>

struct UniformInfo
{
    std::string name;
    GLenum type;
    GLint location;
};

class ShaderProgram
{
public:
    ShaderProgram(GLuint id);
    ~ShaderProgram();

    ShaderProgram(ShaderProgram&& other) noexcept
        : m_id(other.m_id), m_uniforms(std::move(other.m_uniforms))
    {
        other.m_id = 0; // prevent deletion from moved-from object
    }

    // Move assignment
    ShaderProgram& operator=(ShaderProgram&& other) noexcept
    {
        if (this != &other)
        {
            glDeleteProgram(m_id); // clean current
            m_id = other.m_id;
            m_uniforms = std::move(other.m_uniforms);
            other.m_id = 0;
        }
        return *this;
    }

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;


    void Bind();
    void Unbind();

private:
    GLuint m_id;
    std::unordered_map<std::string, UniformInfo> m_uniforms;
};