#pragma once
#include <vector>
#include <glad/gl.h>
#include <stdexcept>

struct VertexBufferElement
{
    GLuint type;
    GLuint count;
    GLboolean normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
            default:
                throw std::runtime_error("Unknown VertexBufferElement type.");
        }
        return 0;
    }
};

class VertexBufferLayout
{
    public:
        VertexBufferLayout():m_stride(0){};

        template<typename T>
        void Push(GLuint count)
        {
            static_assert(false);
        };

        inline const std::vector<VertexBufferElement>& GetElements() const {return m_elements;}
        inline const GLuint GetStride() const {return m_stride;}
    private:
        std::vector<VertexBufferElement> m_elements;
        GLuint m_stride;
};

template<>
inline void VertexBufferLayout::Push<GLfloat>(GLuint count)
{
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
inline void VertexBufferLayout::Push<GLuint>(GLuint count)
{
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
inline void VertexBufferLayout::Push<GLbyte>(GLuint count)
{
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}