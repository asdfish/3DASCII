#include "rendercomponents/VertexArray.hpp"
#include <iostream>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    uintptr_t offset = 0;
    for (unsigned int i = 0; i<elements.size(); i++)
    {
        const auto& element = elements[i];
        std::cout << "Attrib " << i
          << " count: " << element.count
          << " type: " << element.type
          << " normalized: " << element.normalized
          << " stride: " << layout.GetStride()
          << " offset: " << offset << "\n";

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), reinterpret_cast<const void*>(offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }

}

void VertexArray::Bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}