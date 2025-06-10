#pragma once

#include "SceneObject.hpp"

class Scene
{
    public:
        Scene();
        ~Scene();

        std::vector<SceneObject>& GetObjects();
        void AddObject(const SceneObject& object);

        void GetObjectList() const;
        void GetObjectData();
    private:
        std::vector<SceneObject> m_objects;
};
