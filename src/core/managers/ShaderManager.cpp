#include "managers/ShaderManager.hpp"

#include <filesystem>
#include <iostream>

ShaderManager& ShaderManager::Instance()
{
    static ShaderManager s;
    return s;
}

ShaderManager::ShaderManager()
{
    const std::filesystem::path folderPath = "../assets/shaders";
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            std::string name = entry.path().filename().string();
            std::ifstream data(entry.path());
            if (!data) {
                std::cerr << "Failed to open shader file: " << entry.path() << "\n";
                continue;
            }
            std::stringstream buffer;
            buffer << data.rdbuf();
            if (entry.path().extension() == ".vert")
                m_shaders[name] = {GL_VERTEX_SHADER, buffer.str()};
            else if (entry.path().extension() == ".frag")
                m_shaders[name] = {GL_FRAGMENT_SHADER, buffer.str()};
            std::cout<<name<<" has beeen loaded!\n";
        }
    }
}

GLuint ShaderManager::SetupShader(const char *name)
{
    GLuint fs = 0;
    auto it = m_shaders.find(name);
    if (it != m_shaders.end())
    {
        fs = glCreateShader(it->second.type);
        const char* src = it->second.src.c_str();
        glShaderSource(fs, 1, &src, NULL);
        glCompileShader(fs);
        GLint success;
        glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            char infoLog[512];
            glGetShaderInfoLog(fs, 512, NULL, infoLog);
            printf("Shader '%s' compilation failed: %s\n", name, infoLog);
        }
    }
    else
    {
        printf("Shader does not exist\n");
    }

    return fs;
}