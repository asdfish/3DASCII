#pragma once

#include "managers/ComponentManager.hpp"
#include "managers/EntityManager.hpp"
#include "managers/SystemManager.hpp"

class Coordinator
{
    public:
        //Meyers singleton pattern for getting the singleton
        static Coordinator& Instance()
        {
            static Coordinator c;
            return c;
        }

        Coordinator(const Coordinator&) = delete;
        Coordinator& operator = (const Coordinator&) = delete;


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
            signature.set(componentManager->GetComponentType<T>(), true);
            entityManager->SetSignature(entity, signature);
            systemManager->EntitySignatureChanged(entity, signature);
        }

        template <typename T>
        T& GetComponent(Entity entity)
        {
            return componentManager->GetComponent<T>(entity);
        }

        template <typename T>
        ComponentType GetComponentType()
        {
            return componentManager->GetComponentType<T>();
        }


        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            return systemManager->RegisterSystem<T>();
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            systemManager->SetSignature<T>(signature);
        }

        
    private:
        Coordinator() 
        {
            entityManager = std::make_unique<EntityManager>();
            componentManager = std::make_unique<ComponentManager>();
            systemManager = std::make_unique<SystemManager>();
        }
        ~Coordinator() {}
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<SystemManager> systemManager;

};