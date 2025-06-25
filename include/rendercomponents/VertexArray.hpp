#pragma once
#include "rendercomponents/VertexBuffer.hpp"
#include "rendercomponents/VertexBufferLayout.hpp"
#include <memory>

class VertexArray
{
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        void AddBuffer(const std::shared_ptr<VertexBuffer> vb, const VertexBufferLayout& layout);

        unsigned int GetID() const { return m_id; }

        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_id;
};