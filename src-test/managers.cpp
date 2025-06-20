#include <cstdint>
#include <array>
#include <unordered_map>
#include <memory>
#include <bitset>
#include <set>

//Handles creation/destruction of objects
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

template<typename T>
class ComponentStore
{
    public:
        //Add a component of type T to the entity
        void AddData(Entity entity, T component) {
            size_t index = size;
            m_entityToIndex[entity] = index;
            m_indexToEntity[index] = entity;
            m_componentArray[index] = component;
            ++m_size;
        }

        //Remove a component of type T from the entity [TODO: Safety]
        void RemoveData(Entity entity)
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

        //Get the entity's component T
        void GetData(Entity entity)
        {
            return m_componentArray[entityToIndex[entity]];
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

        //Register said component within the componentmanager so it can be accessed later
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
        
        //Add component to an Entuty
        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->AddData(entity, component)
        }
        
        template<typename T>
        T& GetComponent(Entity entity)
        {
            return GetComponentArray<T>->GetData(entity);
        }
    private:
        std::unordered_map<const char*, ComponentType> m_componentTypes;
        std::unordered_map<const char*, std::shared_ptr<void>> m_componentArrays;
        ComponentType nextComponentType = 0;
};

class EntityManager
{
    public:
        Entity CreateEntity()
        {
            return m_nextID++;
        }
        Entity DestroyEntity(Entity entity)
        {
            m_signatures.erase(entity);
        }
        void SetSignature(Entity entity, Signature signature)
        {
            m_signatures[entity] = signature;
        }
        Signature GetSignature(Entity entity)
        {
            return m_signatures[entity];
        }

    private:
        Entity m_nextID = 0;
        std::unordered_map<Entity, Signature> m_signatures;
};

class System
{
    public:
        std::set<Entity> entities;
};

class SystemManager
{
    public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            const char* typeName = type(T).name();
            auto system = std::make_shared<T>;
            m_systems[typeName] = system;
            return system;
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            m_signatures[type(T).name()] = signature;
        }

        void EntityDestroyed(Entity entity)
        {
            for (const auto& pair : m_systems)
            {
                pair.second->entities.erase(entity);
            }
        }

        void EntitySignatureChanged(Entity entity, Signature entitySig)
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

    private:
        std::unordered_map<const char*, Signature> m_signatures;
        std::unordered_map<const char*, std::shared_ptr<System>> m_systems;
};

class Coordinator
{
    public:
        void Init()
        {
            entityManager = std::make_unique<EntityManager>();
            componentManager = std::make_unique<ComponentManager>();
            systemManager = std::make_unique<SystemManager>();
        }
        Entity CreateEntity()
        {
            return entityManager->CreateEntity();
        }

        template <typename T>
        void RegisterComponent()
        {
            componentManager->RegisterComponent<T>();
        }
        template <typename T>
        void AddComponent(Entity entity, T component)
        {
            componentManager->AddComponent<T>(entity, component);
            auto signature = entityManager->GetSignature(entity);
            signature.set(componentManager->GetComponentType<T>, true);
            entityManager->SetSignature(entity, signature);
            systemManager->EntitySignatureChanged(entity, signature)
        }

        template <typename T>
        T& GetComponent(Entity entity)
        {
            return componentManager->GetComponent<T>(entity);
        }

        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            return systemManager->RegisterSystem<T>()
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            systemManager->SetSignature<T>(signature);
        }
    private:
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<SystemManager> systemManager;

};