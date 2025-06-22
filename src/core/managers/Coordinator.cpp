#include "managers/Coordinator.hpp"


Coordinator &Coordinator::Instance()
{
    static Coordinator c;
    return c;
}

Entity Coordinator::CreateEntity()
{
    return entityManager->CreateEntity();
}

template <typename T>
void Coordinator::RegisterComponent()
{
    componentManager->RegisterComponent<T>();
}

template <typename T>
void Coordinator::AddComponent(Entity entity, T component)
{
    componentManager->AddComponent<T>(entity, component);
    auto signature = entityManager->GetSignature(entity);
    signature.set(componentManager->GetComponentType<T>, true);
    entityManager->SetSignature(entity, signature);
    systemManager->EntitySignatureChanged(entity, signature);
}

template <typename T>
T& Coordinator::GetComponent(Entity entity)
{
    return componentManager->GetComponent<T>(entity);
}

template<typename T>
std::shared_ptr<T> Coordinator::RegisterSystem()
{
    return systemManager->RegisterSystem<T>();
}

template<typename T>
void Coordinator::SetSignature(Signature signature)
{
    systemManager->SetSignature<T>(signature);
}
