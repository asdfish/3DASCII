#include "managers/SystemManager.hpp"


template<typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
    const char* typeName = typeid(T).name();
    auto system = std::make_shared<T>;
    m_systems[typeName] = system;
    return system;
}

template<typename T>
void SystemManager::SetSignature(Signature signature)
{
    m_signatures[typeid(T).name()] = signature;
}

void SystemManager::EntityDestroyed(Entity entity)
{
    for (const auto& pair : m_systems)
    {
        pair.second->entities.erase(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySig)
{
    for (const auto& pair:m_systems)
    {
        const auto& systemSig = m_signatures[pair.first];
        if ((entitySig & systemSig) == systemSig)
        {
            pair.second->entities.insert(entity);
        }
        else
        {
            pair.second->entities.erase(entity);
        }
    }
}