#pragma once
#include <glad/gl.h>
#include <fstream>
#include <unordered_map>
#include <memory>

struct ShaderData
{
    GLenum type;
    std::string src;
};

class ShaderManager
{
    public:        
        static ShaderManager& Instance();
        ShaderManager(const ShaderManager&) = delete;
        ShaderManager& operator = (const ShaderManager&) = delete;

        GLuint SetupShader(const char* name);

    private:
        ShaderManager();
        ~ShaderManager() {};
        std::unordered_map<std::string, ShaderData> m_shaders;
        
};