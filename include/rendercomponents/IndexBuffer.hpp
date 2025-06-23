#pragma once
#include <glad/gl.h>

class IndexBuffer
{
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const {return m_count;}
    private:
        GLuint m_id;
        GLuint m_count;

};