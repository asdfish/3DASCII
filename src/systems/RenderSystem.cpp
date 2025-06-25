#include "systems/RenderSystem.hpp"
#include "RenderContext.hpp"
#include "managers/AssetManager.hpp"
#include "errorHandle.hpp"

void RenderSystem::Update()
{
    //std::cout<<entities.size();
    for (const Entity& entity: entities)
    {
        MeshGPU& data = Coordinator::Instance().GetComponent<MeshGPU>(entity);
        Transform& transform = Coordinator::Instance().GetComponent<Transform>(entity);

        data.Bind();

        std::cout<<"data: "<<data.vao->GetID()<<" "<<data.vbo->GetID()<<" "<<data.ibo->GetID()<<" "<<data.count<<"\n";

        GLint boundVao;
        GLCall(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVao));
        GLint boundIbo;
        GLCall(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundIbo));
        GLint boundVbo;
        GLCall(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundVbo));
        std::cout<<"Bound VAO: "<<boundVao<<"\n";
        std::cout<<"Bound IBO: "<<boundIbo<<"\n";
        std::cout<<"Bound VBO: "<<boundVbo<<"\n";

        std::cout<<"Position: "<<transform.position.x<<" "<<transform.position.y<<" "<<transform.position.z<<"\n";

        glm::mat4 world = transform.matrixTransform;
        glm::mat4 view = RenderContext::Instance().viewMatrix;
        glm::mat4 project = RenderContext::Instance().projMatrix;

        glm::vec4 displaypos = project*view*world*glm::vec4(0.f, 0.f, 0.f, 1.f);
        float prevW = displaypos.w;
        displaypos /= prevW;

        std::cout<<"Transformed: "<<displaypos.x<<" "<<displaypos.y<<" "<<displaypos.z<<" "<<displaypos.w<<", prevW = "<<prevW<<"\n";

        AssetManager::Instance().GetShaderProgram("test")->Bind();
        AssetManager::Instance().GetShaderProgram("test")->SetUniformMatrix4fv("worldMatrix", world);
        AssetManager::Instance().GetShaderProgram("test")->SetUniformMatrix4fv("viewMatrix", view);
        AssetManager::Instance().GetShaderProgram("test")->SetUniformMatrix4fv("projMatrix", project);

        
        data.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_INT, nullptr));
        
    }
}