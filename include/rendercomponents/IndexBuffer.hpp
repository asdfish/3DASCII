#pragma once
#include <glad/gl.h>

class IndexBuffer
{
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        inline unsigned int GetID() const {return m_id;}

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const {return m_count;}
    private:
        GLuint m_id;
        GLuint m_count;

};