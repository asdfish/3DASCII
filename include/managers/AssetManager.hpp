#pragma once

#include "rendercomponents.hpp"

#include "components/MeshData.hpp"
#include "components/Transform.hpp"
#include "components/MeshGPU.hpp"
#include "ecss-templates/entity.hpp"
#include "managers.hpp"

#include <glad/gl.h>
#include <fstream>
#include <unordered_map>
#include <map>
#include <memory>
#include <glm/vec4.hpp>

class AssetManager
{
    public:        
        static AssetManager& Instance();
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator = (const AssetManager&) = delete;

        //Shaders
        Shader& GetShader(const std::string& name);
        void CreateShaderProgram(std::string name, std::vector<std::string> names);
        ShaderProgram& GetShaderProgram(const std::string& name);

        //Mesh
        void MeshImport(const char* path);
        void MeshLoad(std::string name);

    private:
        AssetManager();
        ~AssetManager() {};
        glm::vec3 CentreMeshPoints(std::vector<glm::vec4> &points);
        std::unordered_map<std::string, Shader> m_shaders;
        std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;
        std::map<std::string, MeshData> m_meshData;
        GLuint m_shaderProgramID = 0;
        
        
};