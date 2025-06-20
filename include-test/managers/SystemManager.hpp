#pragma once

#include "ecss-templates/signature.hpp"
#include "ecss-templates/entity.hpp"
#include "ecss-templates/system.hpp"

#include <unordered_map>
#include <memory>

class SystemManager
{
    public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem();

        template<typename T>
        void SetSignature(Signature signature);

        void EntityDestroyed(Entity entity);
        void EntitySignatureChanged(Entity entity, Signature entitySig);

    private:
        std::unordered_map<const char*, Signature> m_signatures;
        std::unordered_map<const char*, std::shared_ptr<System>> m_systems;
};