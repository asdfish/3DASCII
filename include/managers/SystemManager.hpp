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
        std::shared_ptr<T> RegisterSystem()
        {
            const char* typeName = typeid(T).name();
            auto system = std::make_shared<T>;
            m_systems[typeName] = system;
            return system;
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            m_signatures[typeid(T).name()] = signature;
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