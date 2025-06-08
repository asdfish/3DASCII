#pragma once

#include "SceneObject.hpp"

class Scene
{
    public:
        Scene();
        ~Scene();

        const std::vector<SceneObject>& GetObjects() const;
        void AddObject(const SceneObject& object);

        void GetObjectList() const;
        void GetObjectData() const;
    private:
        std::vector<SceneObject> m_objects;
};
