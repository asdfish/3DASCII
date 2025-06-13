#pragma once

#include "SceneObject.hpp"

class Scene
{
    public:
        Scene();
        ~Scene();

        std::vector<SceneObject>& GetSceneObjects();
        std::vector<SceneObject>& GetAssetObjects();
        void AddObjectToScene(std::string id);
        void AddObjectToAssets(SceneObject object);

        void GetObjectList() const;
        void GetObjectData();

        bool SaveData();
        bool GetData();
    private:
        std::vector<SceneObject> m_assetObjects; //Objects that can be placed multiple times into the scene.
        std::vector<SceneObject> m_sceneObjects; //Objects present within the scene.
};
