#pragma once

#include "managers/ComponentManager.hpp"
#include "managers/EntityManager.hpp"
#include "managers/SystemManager.hpp"

class Coordinator
{
    public:
        //Meyers singleton pattern for getting the singleton
        static Coordinator& Instance();
        Coordinator(const Coordinator&) = delete;
        Coordinator& operator = (const Coordinator&) = delete;

        Entity CreateEntity();

        template <typename T>
        void RegisterComponent();

        template <typename T>
        void AddComponent(Entity entity, T component);

        template <typename T>
        T& GetComponent(Entity entity);

        template<typename T>
        std::shared_ptr<T> RegisterSystem();

        template<typename T>
        void SetSignature(Signature signature);
        
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