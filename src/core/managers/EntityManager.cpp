#include "managers/EntityManager.hpp"

Entity EntityManager::CreateEntity()
{
    return m_nextID++;
}
void EntityManager::DestroyEntity(Entity entity)
{
    m_signatures.erase(entity);
}
void EntityManager::SetSignature(Entity entity, Signature signature)
{
    m_signatures[entity] = signature;
}
Signature EntityManager::GetSignature(Entity entity)
{
    return m_signatures[entity];
}