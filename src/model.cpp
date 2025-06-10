#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "modelHandling.hpp"
#include "Scene.hpp"

//-------------------------------------------------

Scene::Scene()
{
}

Scene::~Scene()
{
}

const std::vector<SceneObject>& Scene::GetObjects() const
{
    return m_objects;
}

void Scene::AddObject(const SceneObject &object)
{
    m_objects.push_back(object);
}

void Scene::GetObjectList() const
{
    for (std::vector<SceneObject>::const_iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr)
    {
        std::cout<<(*itr).GetName()<<"\n";
    }
}

void Scene::GetObjectData() const
{
    for (std::vector<SceneObject>::const_iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr)
    {
        std::cout<<(*itr).GetName()<<"\n";
        std::cout<<" - Transform Data"<<"\n";
        std::cout<<"Position: "<<(*itr).GetTransform().GetPos().x<<" "<<(*itr).GetTransform().GetPos().y<<" "<<(*itr).GetTransform().GetPos().z<<"\n";
        std::cout<<"Rotation: "<<(*itr).GetTransform().GetRot().x<<" "<<(*itr).GetTransform().GetRot().y<<" "<<(*itr).GetTransform().GetRot().z<<"\n";

        std::cout<<"\n - Model Data - Vertices"<<"\n";
        for (const auto& vert :  (*itr).GetModel().GetVerts())
        {
            std::cout<<(vert).x<<" "<<vert.y<<" "<<vert.z<<"\n";
        }

        std::cout<<"\n - Model Data - Face Indices"<<"\n";
        int count = 0;
        for (const auto& fi :  (*itr).GetModel().GetFaceIndices())
        {
            std::cout<<fi<<" ";
            count = (count+1)%3;
            if (count==0)
            {
                std::cout<< "\n";
            }
        }
    }
}
//------------------------------------------------

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(float3 position, float3 rotation, Model model, std::string name, const Transform* parent)
: m_transform(Transform(position, rotation, parent)), m_model(model), m_name(name)
{

}

SceneObject::~SceneObject()
{
}

const Transform& SceneObject::GetTransform() const
{
    return m_transform;
}

const Model& SceneObject::GetModel() const
{
    return m_model;
}

std::string SceneObject::GetName() const
{
    return m_name;
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

void SceneObject::SetName(std::string name)
{
    m_name = name;
}

//----------------------------------------------------------------

Transform::Transform(float3 pos, float3 rot, const Transform* parent)
: m_pos(pos), m_rot(rot), m_parent(parent)
{
}

Transform::~Transform()
{
}

float3 Transform::GetPos() const
{
    return m_pos;
}

float3 Transform::GetRot() const
{
    return m_rot;
}

const Transform* Transform::GetParent() const
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

//---------------------------------------------------------

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

const std::vector<float3> &Model::GetVerts() const
{
    return m_verts;
}

const std::vector<int> &Model::GetFaceIndices() const
{
    return m_faceIndices;
}

float3 Model::GetPivotFromVerts()
{
    if (static_cast<int>(m_verts.size())== 0)
    {
        return float3();
    }

    float3 output;
    int count = 0;
    for (const auto& vert : m_verts)
    {
        count += 1;
        output.x += vert.x;
        output.y += vert.y;
        output.z += vert.z;
    }
    output.x /= count;
    output.y /= count;
    output.z /= count;
    return output;
}

void Model::AddVert(float3 vert)
{
    m_verts.push_back(vert);
}

void Model::AddFace(int index1, int index2, int index3)
{
    m_faceIndices.push_back(index1);
    m_faceIndices.push_back(index2);
    m_faceIndices.push_back(index3);
}

void Model::AddFaceIndex(int index)
{
    m_faceIndices.push_back(index);
}

//----------------------------------------------------------------

void objImporter(const char* path, Scene& scene)
{
    std::ifstream file(path);
    std::string line;

    std::vector<float3> globalVerts;
    std::vector<std::vector<int>> modelFaceIndices;
    std::vector<std::string> names;

    //Step 1: Parse the file into both the vectors
    while (std::getline(file,line))
    {
        if (line.empty()) continue;

        std::stringstream s(line);
        std::string temp; //keyword
        s>>temp;

        //Object header (o)
        if (temp == "o")
        {
            modelFaceIndices.push_back(std::vector<int>());
            names.push_back(line.substr(2));
        }

        //Vertices
        //Vertex positions (v)
        else if (temp == "v")
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
        //TODO: Make it compatible with quads and n-gons
        //Face Indices (f)
        else if (temp == "f")
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

        SceneObject object(tempModel.GetPivotFromVerts(), float3(), tempModel, names[0]);
        scene.AddObject(object);
        names.erase(names.begin());
    }
}
