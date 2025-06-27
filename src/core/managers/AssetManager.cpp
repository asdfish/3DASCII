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
    if (!file.is_open())  
        throw std::runtime_error("Could not open OBJ at " + std::string(path));

    std::string line;

    std::vector<glm::vec4> globalVertPositons;
    std::vector<glm::vec3> globalVertNormals;

    std::vector<std::vector<int>> modelVertPosIndices;
    std::vector<std::vector<int>> modelVertNormalIndices;

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
            modelVertPosIndices.push_back(std::vector<int>());
            modelVertNormalIndices.push_back(std::vector<int>());
            names.push_back(line.substr(2));
        }

        //Vertices
        //Vertex positions (v)
        else if (temp == "v")
        {
            glm::vec4 newPos;
            s>>temp;
            newPos.x = std::stof(temp);
            s>>temp;
            newPos.y = std::stof(temp);
            s>>temp;
            newPos.z = std::stof(temp);
            newPos.w = 1.0f;

            globalVertPositons.push_back(newPos);
        }

        //Vertex normals (vn)
        else if (temp == "vn")
        {
            glm::vec3 newNormal;
            s>>temp;
            newNormal.x = std::stof(temp);
            s>>temp;
            newNormal.y = std::stof(temp);
            s>>temp;
            newNormal.z = std::stof(temp);

            globalVertNormals.push_back(newNormal);
        }


        //Faces
        //Face Indices (f)
        else if (temp == "f")
        {
            std::vector<int> tempVertPosIndices;
            std::vector<int> tempVertNormalIndices;
            //For now, we are only looking at vertex data, not materials
            while(s>>temp) 
            {
                tempVertPosIndices.push_back(std::stoi(temp.substr(0, temp.find('/')))-1); // -1 for turning array from 1-indexed to 0-indexed

                temp.erase(0, temp.find('/')+1);
                //tempFaceTexIndices.push_back(std::stoi(temp.substr(0, temp.find('/')))-1); //NOTIMPLEMENTED

                temp.erase(0, temp.find('/')+1);
                tempVertNormalIndices.push_back(std::stoi(temp)-1); // -1 for turning array from 1-indexed to 0-indexed
            }
            for (int i = 2; i<tempVertPosIndices.size(); i++)
            {
                modelVertPosIndices.back().push_back(tempVertPosIndices[0]);
                modelVertPosIndices.back().push_back(tempVertPosIndices[i-1]);
                modelVertPosIndices.back().push_back(tempVertPosIndices[i]);

                //TEXUNIMPLEMENTED
                //TEXUNIMPLEMENTED
                //TEXUNIMPLEMENTED

                modelVertNormalIndices.back().push_back(tempVertNormalIndices[0]);
                modelVertNormalIndices.back().push_back(tempVertNormalIndices[i-1]);
                modelVertNormalIndices.back().push_back(tempVertNormalIndices[i]);
            }
        }
    }
    for (int i = 0; i<modelVertPosIndices.size(); i++)
    {
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::unordered_map<IDX, unsigned int, IDXHash> indexGroupMap;

        for (int j = 0; j<modelVertPosIndices[i].size(); j++) //iterator for face (actually for indices of face)
        {
            IDX tempIndices = {modelVertPosIndices[i][j], modelVertNormalIndices[i][j]};
            if (indexGroupMap.find(tempIndices) == indexGroupMap.end()) // Check if the unique combo exists as a vertex already
            {
                indexGroupMap.insert({tempIndices, indexGroupMap.size()});
                vertices.push_back({globalVertPositons[tempIndices.v], globalVertNormals[tempIndices.n] });
            }
            indices.push_back(indexGroupMap[tempIndices]);
        }
        std::string unmodName = names[i];
        std::string name = unmodName;
        int count = 1;

        while (m_meshData.find(name) != m_meshData.end())
        {
            name = unmodName + " (" + std::to_string(count++) + ")";
        }
        m_meshData[name] = {vertices, indices};
    }
}

void AssetManager::MeshLoad(std::string name)
{
    auto data = m_meshData.find(name);
    if (data == m_meshData.end())
    {
        throw std::runtime_error("Model " + name +" does not exist!\n");
    }

    //for (const auto& vertex : data->second.vertices)
    //{
    //    std::cout << "Vertex: " << vertex.x << ", " << vertex.y << ", " << vertex.z << ", " << vertex.w << "\n";
    //}
    //for (const auto& index : data->second.indices)
    //{
    //    std::cout << "Index: " << index << "\n";
    //}

    MeshData dataToLoad = data->second;
    Transform transform = {CentreMeshPoints(dataToLoad.vertices),{1,0,0,0}};
    ID id = {name};
    transform.ResetMatrixTransform();

    if (dataToLoad.vertices.empty() || dataToLoad.indices.empty())
    throw std::runtime_error("Model " + name + " has no vertex or index data.");


    std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();

    std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(dataToLoad.vertices.data(), dataToLoad.vertices.size()*(sizeof(Vertex)));

    VertexBufferLayout layout;
    layout.Push<float>(4);
    layout.Push<float>(3);
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

glm::vec3 AssetManager::CentreMeshPoints(std::vector<Vertex> &points)
{
    glm::vec3 avg(0.f);
    for (const auto& point : points)
    {
        avg += glm::vec3(point.position);
    }
    avg /= points.size();
    for (auto& point : points)
    {
        point.position -= glm::vec4(avg, 0);
    }
    return avg;
}



/*





*/