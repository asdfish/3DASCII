#include "managers/ComponentManager.hpp"
#include "ComponentManager.hpp"

template <typename T>
void ComponentStore<T>::AddData(Entity entity, T component)
{
    size_t index = size;
    m_entityToIndex[entity] = index;
    m_indexToEntity[index] = entity;
    m_componentArray[index] = component;
    ++m_size;
}

template <typename T>
void ComponentStore<T>::RemoveData(Entity entity)
{
    size_t indexOfRemoved = entityToIndex[entity];
    size_t lastIndex = size-1;
    m_componentArray[indexOfRemoved] = m_componentArray[lastIndex];

    Entity lastEntity = m_indexToEntity[lastIndex];
    m_entityToIndex[lastEntity] = indexOfRemoved;
    m_indexToEntity[indexOfRemoved] = lastEntity;
    entityToIndex.erase(entity);
    indexToEntity.erase(lastIndex);
    --size;
}

template <typename T>
void ComponentStore<T>::GetData(Entity entity)
{
    return m_componentArray[entityToIndex[entity]];
}

//---------------------------------------------------------------------------

template<typename T>
void ComponentManager::RegisterComponent()
{
    const char* typeName = typeid(T).name();
    m_componentTypes[typeName] = nextComponentType++;
    m_componentArrays[typeName] = std::make_shared<ComponentStore<T>>();
}

//Get the component ID
template<typename T>
ComponentType ComponentManager::GetComponentType()
{
    return m_componentTypes[typeid(T).name()];
}

//Add component to an Entuty
template<typename T>
void ComponentManager::AddComponent(Entity entity, T component)
{
    GetComponentArray<T>()->AddData(entity, component)
}

template<typename T>
T& ComponentManager::GetComponent(Entity entity)
{
    return GetComponentArray<T>->GetData(entity);
}