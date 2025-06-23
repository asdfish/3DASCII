#include "managers/AssetManager.hpp"
#include "managers/Coordinator.hpp"

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
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            Shader shader = Shader(entry.path().string());
            m_shaders.insert({shader.GetName(), std::move(shader)});
        }
    }
}

Shader& AssetManager::GetShader(const std::string& name)
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
    GLuint id = glCreateProgram();
    for (const auto& name:names)
    {
        GLuint shaderid = GetShader(name).GetID();
        glAttachShader(id, shaderid);
    }
    glLinkProgram(id);
    GLint count;
    glGetProgramiv(id, GL_ATTACHED_SHADERS, &count);

    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        throw std::runtime_error("Shader program linking failed:\n" + std::string(infoLog));
    }
    ShaderProgram shaderprogram(id);
    m_shaderPrograms.insert({progname, std::move(shaderprogram)});
}

ShaderProgram& AssetManager::GetShaderProgram(const std::string& name)
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
    MeshData dataToLoad = m_meshData[name];
    Transform transform = {CentreMeshPoints(dataToLoad.vertices),{0,0,0}};

    VertexArray vao;

    VertexBuffer vbo(dataToLoad.vertices.data(), dataToLoad.vertices.size()*sizeof(glm::vec3));

    VertexBufferLayout layout;
    layout.Push<GLfloat>(4);

    IndexBuffer ibo(dataToLoad.indices.data(), dataToLoad.indices.size() * sizeof(unsigned int));

    vao.AddBuffer(vbo, layout);

    MeshGPU mesh = {vao};

    Entity entity = Coordinator::Instance().CreateEntity();
    Coordinator::Instance().AddComponent<MeshGPU>(entity, mesh);
    Coordinator::Instance().AddComponent<Transform>(entity, transform);
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
        point -= glm::vec4(avg, 1.f);
    }
    return avg;
}