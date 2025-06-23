#include "rendercomponents/ShaderProgram.hpp"
#include <stdexcept>
#include <iostream>

ShaderProgram::ShaderProgram(GLuint id)
: m_id(id)
{
    GLint count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

    for (GLint i = 0; i<count; i++)
    {
        char name[256];
        GLenum type;
        GLint size;
        glGetActiveUniform(id, i, 256, nullptr, &size, &type, name);
        GLint loc = glGetUniformLocation(id, name);
        
        m_uniforms[name] = {name, type, loc};
    }
}

ShaderProgram::~ShaderProgram()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
    }
}

void ShaderProgram::Bind()
{
    glUseProgram(m_id);
}

void ShaderProgram::Unbind()
{
    glUseProgram(0);
}
