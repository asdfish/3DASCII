#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "model.hpp"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(float3 position, float3 rotation, const Transform *parent, Model model)
    : m_transform(Transform(position, rotation, parent)), m_model(model)
{

}

SceneObject::~SceneObject()
{
}

const Transform& SceneObject::GetTransform()
{
    return m_transform;
    // TODO: insert return statement here
}

const Model& SceneObject::GetModel()
{
    return m_model;
    // TODO: insert return statement here
}

void SceneObject::SetTransform(Transform transform)
{
    m_transform = transform;
}

void SceneObject::SetTransform(float3 position, float3 rotation, const Transform *parent)
{
    m_transform.SetPos(position);
    m_transform.SetRot(rotation);
    m_transform.SetParent(parent);
}

void SceneObject::SetModel(Model model)
{
    m_model = model;
}

Transform::Transform(float3 pos, float3 rot, const Transform* parent)
: m_pos(pos), m_rot(rot), m_parent(parent)
{
}

Transform::~Transform()
{
}

const float3& Transform::GetPos()
{
    return m_pos;
}

const float3& Transform::GetRot()
{
    return m_rot;
}

const Transform* Transform::GetParent()
{
    return m_parent;
}

void Transform::SetPos(const float3 &newPos)
{
    m_pos = newPos;
}

void Transform::SetRot(const float3 &newRot)
{
    m_rot = newRot;
}

void Transform::SetParent(const Transform *parent)
{
    m_parent = parent;
}

Model::Model()
{
}

Model::Model(const std::vector<float3> &verts, const std::vector<int> &faceIndices)
    : m_verts(verts), m_faceIndices(faceIndices)
{
}

Model::~Model()
{
}

const std::vector<float3> &Model::GetVerts()
{
    return m_verts;
}

const std::vector<int> &Model::GetFaceIndices()
{
    return m_faceIndices;
}

float3 Model::GetPivotFromVerts()
{
    return float3();
}

void Model::AddVert(float3 vert)
{
}

void Model::AddFace(int index1, int index2, int index3)
{
}

void objImporter(const char *path, Scene& scene)
{
    std::ifstream file("model.obj");
    std::string line;

    std::vector<float3> globalVerts;
    std::vector<std::vector<int>> modelFaceIndices;

    //Step 1: Parse the file into both the vectors
    while (std::getline(file,line))
    {
        if (line.empty()) continue;

        std::stringstream s(line);
        const char* kw; //keyword
        std::string temp;
        s>>temp;
        kw = temp.c_str();

        //Object header (o)
        if (*kw == 'o')
        {
            modelFaceIndices.push_back(std::vector<int>());
            continue;
        }

        //Vertices
        //Vertex positions (v)
        if (*kw == 'v')
        {
            float3 newVert = float3();
            s>>temp;
            newVert.x = std::stof(temp);
            s>>temp;
            newVert.y = std::stof(temp);
            s>>temp;
            newVert.z = std::stof(temp);

            globalVerts.push_back(newVert);
        }

        //Faces
        //Face Indices (f)
        if (*kw == 'f')
        {
            //For now, we are only looking at vertex data, not materials
            while(s>>temp) 
            {
                modelFaceIndices.back().push_back(std::stoi(temp.substr(0, temp.find('/')))-1); // -1 for turning array from 1-indexed to 0-indexed
            }
            
        }
    }

    //Step 2: Turn the parsed data into SceneObjects

    
    for (std::vector<std::vector<int>>::iterator itrm = modelFaceIndices.begin(); itrm != modelFaceIndices.end(); ++itrm) //iterator for model
    {
        Model tempModel;
        std::unordered_map<int, int> indexMap;

        for (std::vector<int>::iterator itrf = (*itrm).begin(); itrf != (*itrm).end(); ++itrf) //iterator for face (actually for indices of face)
        {
            if (indexMap.find(*itrf) == indexMap.end())
            {
                indexMap.insert({*itrf, indexMap.size()});
                tempModel.AddVert(globalVerts[*itrf]);
            }
            tempModel.AddFaceIndex(indexMap[*itrf]);
        }

        SceneObject object(tempModel.GetPivotFromVerts(), float3(), nullptr, tempModel);
        scene.AddObject(object);
    }
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddObject(const SceneObject &object)
{
}
