#include "Model.h"
#include "Mesh.h"
#include "logging.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "stb/stb_image.h"

Model::Model(const std::string& name)
{
    m_name = name;
}

void
Model::Draw(GLuint shaderProgramHandler)
{
    for(auto& mesh : m_meshes){
        mesh.Draw(shaderProgramHandler);
    }
}

void
Model::loadModel(const std::string& path)
{
    if(path.empty()){
        LOG_E(LOGGING_NAME, m_name + " Path is empty, can't process file");
    }

    Assimp::Importer importer;
    // triangulate: automatically transform 3d model faces to triangles
    // flipuvs: go from up-left origin to down-left origin  
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(scene == nullptr){
        LOG_E(LOGGING_NAME, m_name + " Scene failed to load, is nullptr. Extra error: " + importer.GetErrorString());
        return;
    }
    if(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE){
        LOG_E(LOGGING_NAME, m_name + " Scene failed to load, flags incomplete. Extra error: " + importer.GetErrorString());
        return;
    }
    if(scene->mRootNode == nullptr){
        LOG_E(LOGGING_NAME, m_name +  " Scene failed to load, root is nullptr. Extra error: " + importer.GetErrorString());
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void
Model::processNode(const aiNode* node, const aiScene* scene)
{
    if(node == nullptr){
        LOG_E(LOGGING_NAME, m_name + " Can't process node, node is nullptr");
        return;
    }
    if(scene == nullptr){
        LOG_E(LOGGING_NAME, m_name + " Can't process node, scene is nullptr");
        return;
    }

    for(int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        if(mesh == nullptr){
            LOG_E(LOGGING_NAME, m_name + " Can't process null mesh");
            continue;
        }

        std::optional<Mesh> processedMeshResult = processMesh(mesh, scene);
        if(!processedMeshResult.has_value()){
            LOG_E(LOGGING_NAME, m_name + " Mesh processing failed");
            continue;
        }

        m_meshes.push_back(processedMeshResult.value());
    }

    for(int i = 0; i < node->mNumChildren; i++){
        aiNode* childToProcess = node->mChildren[i];
        if(childToProcess == nullptr){
            LOG_E(std::string(LOGGING_NAME) + " Name = " + m_name, "Can't process nullptr child");
            continue;
        }

        processNode(childToProcess, scene);
    }
}

std::optional<Mesh>
Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
    if(mesh == nullptr){
        LOG_E(LOGGING_NAME, m_name + " Can't process nullptr mesh");
        return std::optional<Mesh>();
    }
    if(scene == nullptr){
        LOG_E(LOGGING_NAME, m_name + " Can't process mesh for null scene");
        return std::optional<Mesh>();
    }

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    bool hasTextures = (mesh->mTextureCoords[0] != nullptr);
    if(!hasTextures){
        LOG_I(LOGGING_NAME, m_name + " Model loaded has no textures on channel 0");
    }

    for(int i = 0; i < mesh->mNumVertices; i++){
        Vertex vertex;

        glm::vec3 positionVector;
        auto currentMeshVertex = mesh->mVertices[i];
        positionVector.x = currentMeshVertex.x;
        positionVector.y = currentMeshVertex.y;
        positionVector.z = currentMeshVertex.z;
        vertex.position = positionVector;

        glm::vec3 normalsVector;
        auto currentMeshNormal = mesh->mNormals[i];
        normalsVector.x = currentMeshNormal.x;
        normalsVector.y = currentMeshNormal.y;
        normalsVector.z = currentMeshNormal.z;
        vertex.normal = normalsVector;

        if(hasTextures){
            glm::vec2 texturesVec;

            // [0] is first UV channel: diffuse
            auto currentMeshTextureCoords = mesh->mTextureCoords[0][i];
            texturesVec.x = currentMeshTextureCoords.x;
            texturesVec.y = currentMeshTextureCoords.y;

            vertex.texCoords = texturesVec;
        }
        else{
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for(int i = 0; i < mesh->mNumFaces; i++){
        aiFace currentFace = mesh->mFaces[i];
        for(int j = 0; j < currentFace.mNumIndices; j++){
            indices.push_back(currentFace.mIndices[j]);
        }
    }

    // mesh material index points to array in scene 
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if(material == nullptr){
            LOG_E(LOGGING_NAME, m_name + " Can't process a null material ptr");
        }
        else{
            std::vector<Texture> diffuseMaps  = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            if(diffuseMaps.empty()){
                LOG_W(LOGGING_NAME, m_name + " No diffuse maps found in model");
            }
            if(specularMaps.empty()){
                LOG_W(LOGGING_NAME, m_name + " No specular maps found in model");
            }

            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
    }
    else{
        LOG_I(LOGGING_NAME, m_name + " No materials in current mesh");
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture>
Model::loadMaterialTextures(const aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    if(material == nullptr){
        LOG_E(LOGGING_NAME, m_name + " Can't load material textures, aiMaterial parameter is nullptr");
        return {};
    }

    std::vector<Texture> textures;
    for(int i = 0; i < material->GetTextureCount(type); i++){
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip = false;

        for(int j = 0; j < m_texturesLoaded.size(); j++){
            if(std::strcmp(m_texturesLoaded[j].path.data(), str.C_Str()) == 0){
                textures.push_back(m_texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture newTexture;
            
            newTexture.id = TextureFromFile(str.C_Str(), m_directory);
            newTexture.type = typeName;
            newTexture.path = str.C_Str();
            textures.push_back(newTexture);
            m_texturesLoaded.push_back(newTexture);
        }
    }
    return textures;
}

unsigned int
Model::TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        LOG_E(LOGGING_NAME, m_name + " Failed to load a texture from file " + filename);
        stbi_image_free(data);
    }

    return textureID;
}