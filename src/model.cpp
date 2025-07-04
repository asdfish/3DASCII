#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "json.hpp"
#include "modelHandling.hpp"
#include "Scene.hpp"
#include "model.hpp"
#include "uuid.hpp"
#include "SceneObject.hpp"
#include "Transform.hpp"

//-------------------------------------------------

Scene::Scene()
{
}

Scene::~Scene()
{
}

std::vector<SceneObject>& Scene::GetSceneObjects()
{
    return m_sceneObjects;
}

std::vector<SceneObject> &Scene::GetAssetObjects()
{
    return m_assetObjects;
}

void Scene::AddObjectToScene(std::string id)
{
    for (auto &object : m_assetObjects)
    {
        if (object.GetID() == id)
        {
            object.ChangeID();
            SceneObject newObject = object; // Create a copy of the asset object
            m_sceneObjects.push_back(newObject);
            return;
        }
    }
}

void Scene::AddObjectToScene(SceneObject object)
{
    m_sceneObjects.push_back(object);
}

void Scene::AddObjectToAssets(SceneObject object)
{
    m_assetObjects.push_back(object);
}

bool Scene::RemoveObjectFromScene(const std::string& id)
{
    for (auto itr = m_sceneObjects.begin(); itr != m_sceneObjects.end(); itr++)
    {
        if (itr->GetID() == id)
        {
            m_sceneObjects.erase(itr);
            return true;
        }
    }
    return false;
}

bool Scene::RemoveObjectFromAssets(const std::string& id)
{
    for (auto itr = m_assetObjects.begin(); itr != m_assetObjects.end(); itr++)
    {
        if (itr->GetID() == id)
        {
            m_assetObjects.erase(itr);
            return true;
        }
    }
    return false;
}

void Scene::GetObjectList() const
{
    for (std::vector<SceneObject>::const_iterator itr = m_assetObjects.begin(); itr != m_assetObjects.end(); ++itr)
    {
        std::cout<<(*itr).GetName()<<"\n";
    }
}

void Scene::GetObjectData()
{
    for (std::vector<SceneObject>::iterator itr = m_assetObjects.begin(); itr != m_assetObjects.end(); ++itr)
    {
        std::cout<<(*itr).GetName()<<"\n";
        std::cout<<" - Transform Data"<<"\n";
        std::cout<<"Position: "<<(*itr).GetTransform().GetPos().x<<" "<<(*itr).GetTransform().GetPos().y<<" "<<(*itr).GetTransform().GetPos().z<<"\n";
        std::cout<<"Rotation: "<<(*itr).GetTransform().GetRot().x<<" "<<(*itr).GetTransform().GetRot().y<<" "<<(*itr).GetTransform().GetRot().z<<"\n";

        std::cout<<"\n - Model Data - Vertices"<<"\n";
        for (const auto& vert :  (*itr).GetModel().GetLocalVerts())
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
bool Scene::SaveData(std::vector<Light>& lights, Camera& camera, SceneSettings& settings)
{
    nlohmann::json data;

    //Scene Settings
    data["Scene"]["Settings"]["Lighting Mode"] = static_cast<int>(settings.lightingMode);
    data["Scene"]["Settings"]["Render Mode"] = static_cast<int>(settings.renderMode);
    data["Scene"]["Settings"]["Light Intensity Coefficient"] = settings.lightIntensityCoeff;
    data["Scene"]["Settings"]["Falloff Coefficient"] = settings.falloffCoeff;
    data["Scene"]["Settings"]["Scroll Velocity"]  = settings.scrollVel;
    data["Scene"]["Settings"]["Pan Velocity"] = settings.panVel;
    data["Scene"]["Settings"]["Rotation Velocity"] = settings.rotVel;
    data["Scene"]["Settings"]["FOV Change Speed"] = settings.FOVChangeSpeed;
    data["Scene"]["Settings"]["Motion Velocity"] = settings.motionVel;

    //Camera Position, rotation
    float3 campos = camera.GetTransform().GetPos();
    float3 camrot = camera.GetTransform().GetRot();
    float2 pixelDimensions = camera.GetPixelDimensions();

    data["Scene"]["Camera"]["Position"] = {campos.x, campos.y, campos.z};
    data["Scene"]["Camera"]["Rotation"] = {camrot.x, camrot.y, camrot.z};
    data["Scene"]["Camera"]["FOV"] = camera.GetFOV();
    data["Scene"]["Camera"]["Focal Length"] = camera.GetFocalLength();
    data["Scene"]["Camera"]["Pixel Dimensions"] = {pixelDimensions.x, pixelDimensions.y};

    for (const auto& light : lights)
    {
        std::string id = light.GetID();
        float3 lightpos = light.GetPosition();
        float3 lightcolor = light.GetColor();
        data["Scene"]["Lights"][id]["Name"] = light.GetName();
        data["Scene"]["Lights"][id]["Position"] = {lightpos.x, lightpos.y, lightpos.z};
        data["Scene"]["Lights"][id]["Intensity"] = light.GetIntensity();
        data["Scene"]["Lights"][id]["Color"] = {lightcolor.r, lightcolor.g, lightcolor.b};
    }

    //Scene Objects
    for (auto& itr : m_sceneObjects)
    {
        std::string id = itr.GetID();
        data["Scene"]["Objects"][id]["Name"] = itr.GetName();

        //Transform
        float3 pos = itr.GetTransform().GetPos();
        float3 rot = itr.GetTransform().GetRot();

        // - Position
        data["Scene"]["Objects"][id]["Transform"]["Position"] = {pos.x, pos.y, pos.z};

        // - Rotation
        data["Scene"]["Objects"][id]["Transform"]["Rotation"] = {rot.x, rot.y, rot.z};

        //Model
        std::vector<float3> verts = itr.GetModel().GetLocalVerts();
        std::vector<std::vector<float>> storedVerts;
        for (const auto& v : verts)
        {
            storedVerts.push_back({ v.x, v.y, v.z });
        }

        std::vector<int> faceIndices = itr.GetModel().GetFaceIndices();
        float3 modelcolor = itr.GetModel().GetColor();

        //Vertices
        data["Scene"]["Objects"][id]["Model"]["Vertices"] = storedVerts;

        //Face Indices
        data["Scene"]["Objects"][id]["Model"]["Face Indices"] = faceIndices;

        //Model Color
        data["Scene"]["Objects"][id]["Model"]["Color"] = {modelcolor.r, modelcolor.g, modelcolor.b};
    }

    //Asset Objects
    for (auto& itr : m_assetObjects)
    {
        std::string id = itr.GetID();
        data["Asset"]["Objects"][id]["Name"] = itr.GetName();

        //Transform
        float3 pos = itr.GetTransform().GetPos();
        float3 rot = itr.GetTransform().GetRot();

        // - Position
        data["Asset"]["Objects"][id]["Transform"]["Position"] = {pos.x, pos.y, pos.z};

        // - Rotation
        data["Asset"]["Objects"][id]["Transform"]["Rotation"] = {rot.x, rot.y, rot.z};

        //Model
        std::vector<float3> verts = itr.GetModel().GetLocalVerts();
        std::vector<std::vector<float>> storedVerts;
        for (const auto& v : verts)
        {
            storedVerts.push_back({ v.x, v.y, v.z });
        }

        std::vector<int> faceIndices = itr.GetModel().GetFaceIndices();

        //Vertices
        data["Asset"]["Objects"][id]["Model"]["Vertices"] = storedVerts;

        //Face Indices
        data["Asset"]["Objects"][id]["Model"]["Face Indices"] = faceIndices;
    }

    std::ofstream outFile("saveData.json");

    if (!outFile.is_open())
        return false;

    outFile << data.dump(4);
    return true;
}
bool Scene::GetData(std::vector<Light>& lights, Camera& camera, SceneSettings& settings)
{
    nlohmann::json data;
    std::ifstream file("saveData.json");

    if(!file.good())
    {
        return false;
    }

    file >> data;

    //Scene Settings
    settings.lightingMode = static_cast<LightingMode>(data["Scene"]["Settings"]["Lighting Mode"].get<int>());
    settings.renderMode = static_cast<RenderMode>(data["Scene"]["Settings"]["Render Mode"].get<int>());
    settings.lightIntensityCoeff = data["Scene"]["Settings"]["Light Intensity Coefficient"];
    settings.falloffCoeff = data["Scene"]["Settings"]["Falloff Coefficient"];
    settings.scrollVel = data["Scene"]["Settings"]["Scroll Velocity"];
    settings.panVel = data["Scene"]["Settings"]["Pan Velocity"];
    settings.rotVel = data["Scene"]["Settings"]["Rotation Velocity"];
    settings.FOVChangeSpeed = data["Scene"]["Settings"]["FOV Change Speed"];
    settings.motionVel = data["Scene"]["Settings"]["Motion Velocity"];

    //Camera
    std::vector<float> campos = data["Scene"]["Camera"]["Position"];
    std::vector<float> camrot = data["Scene"]["Camera"]["Rotation"];
    float fov = data["Scene"]["Camera"]["FOV"];
    float focalLength = data["Scene"]["Camera"]["Focal Length"];
    std::vector<float> pixelDimensions = data["Scene"]["Camera"]["Pixel Dimensions"];

    //Lights
    for (auto& [id, lightData] : data["Scene"]["Lights"].items())
    {
        std::vector<float> lightpos = lightData["Position"];
        std::vector<float> lightcol = lightData["Color"];
        lights.emplace_back(float3(lightpos[0], lightpos[1], lightpos[2]), lightData["Intensity"], lightData["Name"], id, float3(lightcol[0], lightcol[1], lightcol[2]));
    }

    camera = Camera(float3(campos[0], campos[1],campos[2]), float3(camrot[0], camrot[1],camrot[2]), fov, focalLength, pixelDimensions[0], pixelDimensions[1]);


    for (auto& [id, objectData] : data["Scene"]["Objects"].items())
    {
        
        std::string name = objectData["Name"];

        //Model
        std::vector<std::vector<float>> storedVerts = objectData["Model"]["Vertices"];
        std::vector<float3> verts;
        std::vector<float> modelColor = objectData["Model"]["Color"]; 
        
        for (const auto& v : storedVerts)
        {
            verts.emplace_back(v[0], v[1], v[2]);
        }

        std::vector<int> faceIndices = objectData["Model"]["Face Indices"];
        

        Model model(verts, faceIndices, float3(modelColor[0],modelColor[1],modelColor[2]));

        //Transform
        float3 pos = float3(objectData["Transform"]["Position"][0], objectData["Transform"]["Position"][1], objectData["Transform"]["Position"][2]);
        float3 rot = float3(objectData["Transform"]["Rotation"][0], objectData["Transform"]["Rotation"][1], objectData["Transform"]["Rotation"][2]);

        m_sceneObjects.emplace_back(pos, rot, model, name, id);
    }

    for (auto& [id, objectData] : data["Asset"]["Objects"].items())
    {
        
        std::string name = objectData["Name"];

        //Model
        std::vector<std::vector<float>> storedVerts = objectData["Model"]["Vertices"];
        std::vector<float3> verts;
        
        for (const auto& v : storedVerts)
        {
            verts.emplace_back(v[0], v[1], v[2]);
        }

        std::vector<int> faceIndices = objectData["Model"]["Face Indices"];

        Model model(verts, faceIndices, float3(1,1,1));

        //Transform
        float3 pos = float3(objectData["Transform"]["Position"][0], objectData["Transform"]["Position"][1], objectData["Transform"]["Position"][2]);
        float3 rot = float3(objectData["Transform"]["Rotation"][0], objectData["Transform"]["Rotation"][1], objectData["Transform"]["Rotation"][2]);

        m_assetObjects.emplace_back(pos, rot, model, name, id);
    }
    return true;
}

//------------------------------------------------

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(float3 position, float3 rotation, Model model, std::string name, Transform* parent)
: m_transform(Transform(position, rotation, parent)), m_model(model), m_name(name)
{
    m_id = uuid::generate_uuid_v4();
}

SceneObject::SceneObject(float3 position, float3 rotation, Model model, std::string name, std::string id, Transform *parent)
: m_transform(Transform(position, rotation, parent)), m_model(model), m_name(name), m_id(id)
{
}

SceneObject::~SceneObject()
{
}

Transform& SceneObject::GetTransform()
{
    return m_transform;
}

Model& SceneObject::GetModel()
{
    return m_model;
}

std::string SceneObject::GetName() const
{
    return m_name;
}

std::string SceneObject::GetID() const
{
    return m_id;
}

void SceneObject::SetTransform(Transform transform)
{
    m_transform = transform;
}

void SceneObject::SetTransform(float3 position, float3 rotation, Transform *parent)
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

void SceneObject::ChangeID()
{
    m_id = uuid::generate_uuid_v4();
}
//----------------------------------------------------------------

Transform::Transform(float3 pos, float3 rot, Transform* parent)
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

Transform* Transform::GetParent()
{
    return m_parent;
}

float3 Transform::GetForwardVector() const
{
    return float3(
        cos(m_rot.x) * sin(m_rot.y), 
        -sin(m_rot.x), 
        cos(m_rot.x) * cos(m_rot.y));
}
float3 Transform::GetRightVector() const
{
    return float3(
        cos(m_rot.y), 
        0, 
        -sin(m_rot.y));
}
float3 Transform::GetUpVector() const
{
    return float3(
    sin(m_rot.x) * sin(m_rot.y), 
    cos(m_rot.x), 
    sin(m_rot.x) * cos(m_rot.y)) * -1;
}
void Transform::SetPos(const float3 &newPos)
{
    m_pos = newPos;
}

void Transform::SetRot(const float3 &newRot)
{
    m_rot = newRot;
}

void Transform::SetParent(Transform* parent)
{
    m_parent = parent;
}

//---------------------------------------------------------

Model::Model()
    : m_verts(), m_faceIndices(), m_color(1,1,1)
{
}

Model::Model(const std::vector<float3> &verts, const std::vector<int> &faceIndices, float3 color)
    : m_verts(verts), m_faceIndices(faceIndices) , m_color(color)
{
}

Model::~Model()
{
}

float3 Rotate(float3 v, float pitch, float yaw, float roll) {
    // Convert to radians
    float cx = std::cos(pitch), sx = std::sin(pitch);
    float cy = std::cos(yaw),   sy = std::sin(yaw);
    float cz = std::cos(roll),  sz = std::sin(roll);

    // Combined rotation matrix (Roll * Pitch * Yaw)
    float3 result;
    result.x = v.x * (cy * cz + sy * sx * sz) + v.y * (sz * cx) + v.z * (-sy * cz + cy * sx * sz);
    result.y = v.x * (-cy * sz + sy * sx * cz) + v.y * (cz * cx) + v.z * (sz * sy + cy * sx * cz);
    result.z = v.x * (sy * cx) + v.y * (-sx) + v.z * (cy * cx);

    return result;
}

//Returns the vertices of the model in global space
std::vector<float3> Model::GetVerts(const float3& position, const float3& rotation) const
{
    std::vector<float3> globalVerts;
    float3 tempVert;
    globalVerts.reserve(m_verts.size());
    for (const auto& vert : m_verts)
    {
        tempVert = Rotate(vert, rotation.x, rotation.y, rotation.z); // Apply rotation
        globalVerts.push_back(float3(tempVert.x + position.x, tempVert.y + position.y, tempVert.z + position.z));
    }

    return globalVerts;
}


const std::vector<float3> &Model::GetLocalVerts() const
{
    return m_verts;
}
const std::vector<int> &Model::GetFaceIndices() const
{
    return m_faceIndices;
}

const float3 &Model::GetColor() const
{
    return m_color;
}

//Recalculate coordinates of all points from the average of all vertices. Also returns said average.
float3 Model::InitializePositionsFromPivot()
{
    if (m_verts.empty())
    {
        return float3();
    }

    float3 pivot;

    for (const auto& vert : m_verts)
    {
        pivot.x += vert.x;
        pivot.y += vert.y;
        pivot.z += vert.z;
    }

    const float invCount = 1/(static_cast<float>(m_verts.size()));
    pivot.x *= invCount;
    pivot.y *= invCount;
    pivot.z *= invCount;

    for (auto& vert : m_verts)
    {
        vert.x -= pivot.x;
        vert.y -= pivot.y;
        vert.z -= pivot.z;
    }

    return pivot;
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

void Model::SetColor(const float3 &color)
{
    m_color = color;
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
            std::vector<int> tempFaceIndices;
            //For now, we are only looking at vertex data, not materials
            while(s>>temp) 
            {
                tempFaceIndices.push_back(std::stoi(temp.substr(0, temp.find('/')))-1); // -1 for turning array from 1-indexed to 0-indexed
            }
            for (int i = 2; i<tempFaceIndices.size(); i++)
            {
                modelFaceIndices.back().push_back(tempFaceIndices[0]);
                modelFaceIndices.back().push_back(tempFaceIndices[i-1]);
                modelFaceIndices.back().push_back(tempFaceIndices[i]);
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

        float3 pivot = tempModel.InitializePositionsFromPivot(); 

        SceneObject object(pivot, float3(), tempModel, names[0]);
        scene.AddObjectToAssets(object);
        names.erase(names.begin());
    }
}
