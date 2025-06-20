#pragma once

#include "managers/ComponentManager.hpp"
#include "managers/EntityManager.hpp"
#include "managers/SystemManager.hpp"

class Coordinator
{
    public:
        void Init();
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
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<SystemManager> systemManager;

};