#pragma once
#include "rendercomponents/VertexBuffer.hpp"
#include "rendercomponents/VertexBufferLayout.hpp"

class VertexArray
{
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_id;
};