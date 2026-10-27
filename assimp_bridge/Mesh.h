#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/ext/matrix_float4x4.hpp"
#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture
{
    unsigned int id;
    std::string type; // type of texture, for example diffuse or specular
    std::string path;
};

class Mesh
{
private:
    static constexpr const char* const LOGGING_NAME = "assimp_bridge::Mesh";

public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices, const std::vector<Texture>& textures);
    void Draw(GLuint &shaderProgramHandler);

public:
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture>      m_textures;
    
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setupMesh();
};

// uniform sampler2D texture_diffuse0;
// uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_diffuse2;
// uniform sampler2D texture_specular0;
// uniform sampler2D texture_specular1;
