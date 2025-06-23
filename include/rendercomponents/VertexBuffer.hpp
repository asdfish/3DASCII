#pragma once
#include <glad/gl.h>

class VertexBuffer
{
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
    private:
        GLuint m_id;

};