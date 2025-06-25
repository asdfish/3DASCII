#pragma once
#include <glad/gl.h>

class VertexBuffer
{
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        inline unsigned int GetID() const { return m_id; }

        void Bind() const;
        void Unbind() const;
    private:
        GLuint m_id;

};