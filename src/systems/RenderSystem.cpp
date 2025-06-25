#define GLM_ENABLE_EXPERIMENTAL

#include "systems/RenderSystem.hpp"
#include "RenderContext.hpp"
#include "managers/AssetManager.hpp"
#include "errorHandle.hpp"
#include "glm/gtx/string_cast.hpp"

void RenderSystem::Update()
{
    //std::cout<<entities.size();
    for (const Entity& entity: entities)
    {
        MeshGPU& data = Coordinator::Instance().GetComponent<MeshGPU>(entity);
        Transform& transform = Coordinator::Instance().GetComponent<Transform>(entity);

        data.Bind();

        glm::mat4 world = transform.matrixTransform;
        glm::mat4 view = RenderContext::Instance().viewMatrix;
        glm::mat4 project = RenderContext::Instance().projMatrix;

        AssetManager::Instance().GetShaderProgram("test")->Bind();
        AssetManager::Instance().GetShaderProgram("test")->SetUniformMatrix4fv("worldMatrix", world);
        AssetManager::Instance().GetShaderProgram("test")->SetUniformMatrix4fv("viewMatrix", view);
        AssetManager::Instance().GetShaderProgram("test")->SetUniformMatrix4fv("projMatrix", project);

        
        data.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_INT, nullptr));
    }
}


/*



        float points[] = {
            1.f, 1.f, -1.f, 1.0f,
            1.f, -1.f, -1.f, 1.0f,
            1.f, 1.f, 1.f, 1.0f,
            1.f, -1.f, 1.f, 1.0f,
            -1.f, 1.f, -1.f, 1.0f,
            -1.f, -1.f, -1.f, 1.0f,
            -1.f, 1.f, 1.f, 1.0f,
            -1.f, -1.f, 1.f, 1.0f,
        };

        GLuint indices[] = {
            4, 2, 0,
            2, 7, 3,
            6, 5, 7,
            1, 7, 5,
            0, 3, 1,
            4, 1, 5,
            4, 6, 2,
            2, 6, 7,
            6, 4, 5,
            1, 3, 7,
            0, 2, 3,
            4, 0, 1
        };

        std::shared_ptr<VertexArray> va = std::make_shared<VertexArray>();
        std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(points, 4*8*sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(4);
        va->AddBuffer(vb, layout);

        std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(indices, 36);
        va->Bind();

*/