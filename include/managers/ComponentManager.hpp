#pragma once

#include <unordered_map>
#include <memory>

#include "ecss-templates/component.hpp"
#include "ecss-templates/entity.hpp"

template<typename T>
class ComponentStore
{
    public:
        //Add a component of type T to the entity
        void AddData(Entity entity, T component);

        //Remove a component of type T from the entity [TODO: Safety]
        void RemoveData(Entity entity);

        //Get the entity's component T
        void GetData(Entity entity);
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

        //Register said component within the componentmanager so it can be accessed later
        template<typename T>
        void RegisterComponent();
        
        //Get the component ID
        template<typename T>
        ComponentType GetComponentType();

        template <typename T>
        std::shared_ptr<T> GetComponentArray();
        
        //Add component to an Entity
        template<typename T>
        void AddComponent(Entity entity, T component);
        
        template<typename T>
        T& GetComponent(Entity entity);

    private:
        std::unordered_map<const char*, ComponentType> m_componentTypes;
        std::unordered_map<const char*, std::shared_ptr<void>> m_componentArrays;
        ComponentType nextComponentType = 0;
};