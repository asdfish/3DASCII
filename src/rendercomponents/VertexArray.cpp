#include "rendercomponents/VertexArray.hpp"
#include "errorHandle.hpp"
#include <iostream>

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_id));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_id));
    m_id = 0;
}

void VertexArray::AddBuffer(const std::shared_ptr<VertexBuffer> vb, const VertexBufferLayout& layout)
{
    Bind();
    vb->Bind();
    const auto& elements = layout.GetElements();
    uintptr_t offset = 0;
    for (unsigned int i = 0; i<elements.size(); i++)
    {
        const auto& element = elements[i];

        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), reinterpret_cast<const void*>(offset)));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }

}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}