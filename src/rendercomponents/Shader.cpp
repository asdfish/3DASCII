#include "rendercomponents/Shader.hpp"
#include "errorHandle.hpp"
#include <iostream>
#include "errhandlingapi.h"

Shader::Shader(const std::string &filepath)
: m_id(0), m_filePath(filepath)
{
    std::string data = ParseShader(filepath);
    m_id = CompileShader(data);
}

Shader::~Shader()
{
    if (m_id != 0)
        glDeleteShader(m_id);
}

std::string Shader::ParseShader(const std::string& filepath)
{
    std::filesystem::path path = std::filesystem::u8path(filepath);
    std::string src;
    std::ifstream data(path);

    if (!data) {
       throw std::runtime_error("Failed to open shader file: " + filepath);
    }

    std::stringstream buffer;
    buffer << data.rdbuf();

    if (path.extension().string() == ".vert") m_type = GL_VERTEX_SHADER;
    else if (path.extension().string() == ".frag") m_type = GL_FRAGMENT_SHADER;
    else throw std::runtime_error("Unsupported shader extension: " + path.extension().string());

    m_name = path.filename().string();
    m_filePath = filepath;
    src = buffer.str();

    std::cout<<m_name<<" has beeen parsed!\n";
    std::cout<<src<<"\n";

    return src;
}

GLuint Shader::CompileShader(std::string src)
{
    GLCall(GLuint s = glCreateShader(m_type));

    if (s == 0) 
    {
        throw std::runtime_error("Failed to create shader object");
    }

    const char* source = src.c_str();
    GLCall(glShaderSource(s, 1, &source, NULL));
    GLCall(glCompileShader(s));
    GLint success;
    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(s, 512, NULL, infoLog);
        throw std::runtime_error("Shader '%s' compilation failed: %s\n" + m_name + infoLog);
    }
    std::cout<<m_name<<" has beeen compiled!\n";
    return s;
}

GLuint Shader::GetUniformLocation(const std::string &name)
{
    return glGetUniformLocation(m_id, name.c_str());
}

