#pragma once

#include <unordered_map>
#include <string>
#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

struct UniformInfo
{
    std::string name;
    GLenum type;
    GLint size;
    GLint location;
};

class ShaderProgram
{
public:
    ShaderProgram(GLuint id);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void SetUniform4f(const std::string& name, const glm::vec4& val);
    void SetUniform3f(const std::string& name, const glm::vec3& val);
    void SetUniform2f(const std::string& name, const glm::vec2& val);
    void SetUniform1f(const std::string& name, float val);
    void SetUniformMatrix4fv(const std::string& name, const glm::mat4& val);

    void Bind();
    void Unbind();

private:
    GLuint m_id;
    std::unordered_map<std::string, UniformInfo> m_uniforms;
};