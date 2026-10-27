#pragma once
#include "Mesh.h"
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>      
#include <assimp/postprocess.h>

class Model
{
private:
    static constexpr const char* const LOGGING_NAME = "assimp_bridge::Model";

public:
    Model(const std::string& name);

    void Draw(GLuint shaderProgramHandler);
    void loadModel(const std::string& path);

private: 
    void processNode(const aiNode* node, const aiScene* scene);
    std::optional<Mesh> processMesh(const aiMesh*  mes, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial* material, aiTextureType type, const std::string& typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);

private:
    std::vector<Mesh>    m_meshes;
    std::vector<Texture> m_texturesLoaded;
    std::string          m_directory;
    std::string          m_name;
};

