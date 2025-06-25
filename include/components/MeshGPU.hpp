#pragma once

#include <glad/gl.h>
#include <memory>
#include  "rendercomponents/VertexArray.hpp"

struct MeshGPU
{
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<IndexBuffer> ibo;
    GLuint count;

    void Bind()
    {
        vao->Bind();
        ibo->Bind();
    }
    void Unbind()
    {
        ibo->Unbind();
        vao->Unbind();
    }
};