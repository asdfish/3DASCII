#include "managers/AssetManager.hpp"
#include "managers/Coordinator.hpp"
#include "errorHandle.hpp"

#include <filesystem>
#include <iostream>

AssetManager& AssetManager::Instance()
{
    static AssetManager s;
    return s;
}

AssetManager::AssetManager()
{
    const std::filesystem::path folderPath = "../assets/shaders";

    if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath))
    throw std::runtime_error("Shader folder not found: " + folderPath.string());

    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            m_shaders.insert({entry.path().filename().string(), std::make_shared<Shader>(entry.path().string())});
        }
    }
}

std::shared_ptr<Shader> AssetManager::GetShader(const std::string& name)
{
    auto it = m_shaders.find(name);
    if (it != m_shaders.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Shader '" + name + "' does not exist\n");
    }
}

void AssetManager::CreateShaderProgram(std::string progname, std::vector<std::string> names)
{
    GLCall(GLuint id = glCreateProgram());
    std::vector<std::shared_ptr<Shader>> attachedShaders;
    for (const auto& name:names)
    {
        auto shader = GetShader(name);
        attachedShaders.push_back(shader);
        GLCall(glAttachShader(id, shader->GetID()));
    }
    GLCall(glLinkProgram(id));

    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        throw std::runtime_error("Shader program linking failed:\n" + std::string(infoLog));
    }
    m_shaderPrograms.insert({progname, std::make_shared<ShaderProgram>(id)});
}

std::shared_ptr<ShaderProgram> AssetManager::GetShaderProgram(const std::string& name)
{
    auto itr = m_shaderPrograms.find(name);
    if (itr != m_shaderPrograms.end())
        return itr->second;
    throw std::runtime_error("Shader Program '" + name + "' doesn't exist.");
}

void AssetManager::MeshImport(const char* path)
{
    std::ifstream file(path);
    std::string line;

    std::vector<glm::vec4> globalVerts;
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
            glm::vec4 newVert;
            s>>temp;
            newVert.x = std::stof(temp);
            s>>temp;
            newVert.y = std::stof(temp);
            s>>temp;
            newVert.z = std::stof(temp);
            newVert.w = 1.0f;

            globalVerts.push_back(newVert);
        }

        //Faces
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
    for (auto itrm = modelFaceIndices.begin(); itrm != modelFaceIndices.end(); ++itrm)
    {
        
        std::vector<glm::vec4> vertices;
        std::vector<unsigned int> faceIndices;
        std::unordered_map<int, int> indexMap;

        for (auto itrf = itrm->begin(); itrf != itrm->end(); ++itrf) //iterator for face (actually for indices of face)
        {
            if (indexMap.find(*itrf) == indexMap.end())
            {
                indexMap.insert({*itrf, indexMap.size()});
                vertices.push_back(globalVerts[*itrf]);
            }
            faceIndices.push_back(indexMap[*itrf]);
        }
        std::string unmodName = names[0];
        std::string name = unmodName;
        int count = 1;

        while (m_meshData.find(name) != m_meshData.end())
        {
            name = unmodName + " (" + std::to_string(count++) + ")";
        }
        m_meshData[name] = {vertices, faceIndices};

       
        names.erase(names.begin());
    }
}

void AssetManager::MeshLoad(std::string name)
{
    auto data = m_meshData.find(name);
    if (data == m_meshData.end())
    {
        throw std::runtime_error("Model " + name +" does not exist!\n");
    }

    for (const auto& vertex : data->second.vertices)
    {
        std::cout << "Vertex: " << vertex.x << ", " << vertex.y << ", " << vertex.z << ", " << vertex.w << "\n";
    }
    for (const auto& index : data->second.indices)
    {
        std::cout << "Index: " << index << "\n";
    }

    MeshData dataToLoad = data->second;
    Transform transform = {CentreMeshPoints(dataToLoad.vertices),{1,0,0,0}};
    ID id = {name};
    transform.ResetMatrixTransform();

    if (dataToLoad.vertices.empty() || dataToLoad.indices.empty())
    throw std::runtime_error("Model " + name + " has no vertex or index data.");


    std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();

    std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(dataToLoad.vertices.data(), dataToLoad.vertices.size()*sizeof(glm::vec4));

    VertexBufferLayout layout;
    layout.Push<float>(4);
    vao->AddBuffer(vbo, layout);

    std::shared_ptr<IndexBuffer> ibo = std::make_shared<IndexBuffer>(dataToLoad.indices.data(), dataToLoad.indices.size());
    vao->Bind();
    ibo->Bind();

    MeshGPU mesh = {vao, vbo, ibo, ibo->GetCount()};

    Entity entity = Coordinator::Instance().CreateEntity();
    Coordinator::Instance().AddComponent<MeshGPU>(entity, mesh);
    Coordinator::Instance().AddComponent<Transform>(entity, transform);
    Coordinator::Instance().AddComponent<ID>(entity, id);
}

std::vector<std::string> AssetManager::GetMeshNames()
{
    std::vector<std::string> names;
    names.reserve(m_meshData.size());
    for (const auto& data : m_meshData)
    {
        names.push_back(data.first);
    }
    return names;
}

void AssetManager::RemoveMesh(std::string name)
{
    m_meshData.erase(name);
}

glm::vec3 AssetManager::CentreMeshPoints(std::vector<glm::vec4> &points)
{
    glm::vec3 avg(0.f);
    for (const auto& point : points)
    {
        avg += glm::vec3(point);
    }
    avg /= points.size();
    for (auto& point : points)
    {
        point -= glm::vec4(avg, 0);
    }
    return avg;
}