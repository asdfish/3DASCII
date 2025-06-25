#include "rendercomponents/ShaderProgram.hpp"
#include "errorHandle.hpp"
#include <stdexcept>
#include <iostream>


ShaderProgram::ShaderProgram(GLuint id)
: m_id(id)
{
    GLint count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
    //std::cout<<count<<"\n";

    for (GLint i = 0; i<count; i++)
    {
        char name[256];
        GLenum type;
        GLint size;
        glGetActiveUniform(id, i, 256, nullptr, &size, &type, name);
        GLint loc = glGetUniformLocation(id, name);
        //std::cout<<name<<" "<<type<<" "<<size<<" "<<loc<<"\n";
        m_uniforms[name] = {name, type, size, loc};
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

void ShaderProgram::SetUniform4f(const std::string &name, const glm::vec4 val)
{
    auto uniform = m_uniforms.find(name);

    if (uniform              != m_uniforms.end() 
    &&  uniform->second.type == GL_FLOAT_VEC4
    &&  uniform->second.size == 4               )
    {
        GLCall(glUniform4f(uniform->second.location, val[0], val[1], val[2], val[3]));
    }
    else
    {
        throw std::runtime_error("Invalid value passed to SetUniform4f : " + name + " does not exist or is of incompatible type!");
    }
}

void ShaderProgram::SetUniformMatrix4fv(const std::string &name, const glm::mat4 val)
{
    auto uniform = m_uniforms.find(name);

    if (uniform              != m_uniforms.end() 
    &&  uniform->second.type == GL_FLOAT_MAT4
    &&  uniform->second.size == 1               )
    {
        //std::cout<<name<<" "<<uniform->second.name<<" "<<uniform->second.type<<" "<<uniform->second.size<<" "<<uniform->second.location<<"\n";
        GLCall(glUniformMatrix4fv(uniform->second.location, 1, false, glm::value_ptr(val)));
    }
    else
    {
        throw std::runtime_error("Invalid value passed to SetUniformMatrix4fv : " + name + " does not exist or is of incompatible type!");
    }
}
