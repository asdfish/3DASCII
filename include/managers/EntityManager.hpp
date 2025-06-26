#pragma once

#include "ecss-templates/entity.hpp"
#include "ecss-templates/signature.hpp"

#include <unordered_map>

class EntityManager
{
    public:
        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        void SetSignature(Entity entity, Signature signature);
        Signature GetSignature(Entity entity);

    private:
        Entity m_nextID = 1;
        std::unordered_map<Entity, Signature> m_signatures;
};