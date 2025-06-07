#pragma once

#include "SceneObject.hpp"

class Scene
{
    public:
        Scene();
        ~Scene();

        void AddObject(const SceneObject& object);
        void GetObjectList() const;
        void GetObjectData() const;
    private:
        std::vector<SceneObject> m_objects;
};
