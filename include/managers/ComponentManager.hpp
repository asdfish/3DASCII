#pragma once

#include <unordered_map>
#include <memory>

#include "ecss-templates/component.hpp"
#include "ecss-templates/entity.hpp"

template<typename T>
class ComponentStore
{
    public:
        void AddData(Entity entity, T component)
        {
            size_t index = m_size;
            m_entityToIndex[entity] = index;
            m_indexToEntity[index] = entity;
            m_componentArray[index] = component;
            ++m_size;
        }

        void RemoveData(Entity entity)
        {
            size_t indexOfRemoved = m_entityToIndex[entity];
            size_t lastIndex = m_size-1;
            m_componentArray[indexOfRemoved] = m_componentArray[lastIndex];

            Entity lastEntity = m_indexToEntity[lastIndex];
            m_entityToIndex[lastEntity] = indexOfRemoved;
            m_indexToEntity[indexOfRemoved] = lastEntity;
            m_entityToIndex.erase(entity);
            m_indexToEntity.erase(lastIndex);
            --m_size;
        }

        T& GetData(Entity entity)
        {
            return m_componentArray[m_entityToIndex[entity]];
        }
    private:
        std::array<T, MAX_ENTITIES> m_componentArray;
        std::unordered_map<Entity, size_t> m_entityToIndex;
        std::unordered_map<size_t, Entity> m_indexToEntity;
        size_t m_size = 0;
};

//Component Manager Class
class ComponentManager
{
    public:
        template<typename T>
        void RegisterComponent()
        {
            const char* typeName = typeid(T).name();
            m_componentTypes[typeName] = nextComponentType++;
            m_componentArrays[typeName] = std::make_shared<ComponentStore<T>>();
        }

        //Get the component ID
        template<typename T>
        ComponentType GetComponentType()
        {
            return m_componentTypes[typeid(T).name()];
        }

        template<typename T>
        std::shared_ptr<ComponentStore<T>> GetComponentArray()
        {
            return std::static_pointer_cast<ComponentStore<T>>(m_componentArrays[typeid(T).name()]);
        }

        //Add component to an Entuty
        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->AddData(entity, component);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return GetComponentArray<T>()->GetData(entity);
        }

    private:
        std::unordered_map<const char*, ComponentType> m_componentTypes;
        std::unordered_map<const char*, std::shared_ptr<void>> m_componentArrays;
        ComponentType nextComponentType = 0;
};