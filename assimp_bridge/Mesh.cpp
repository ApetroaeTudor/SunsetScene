#include "Mesh.h"
#include "logging.h"
#include <GL/gl.h>
#include <string>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices, const std::vector<Texture>& textures)
{
    this->m_vertices = vertices;
    this->m_indices  = indices;
    this->m_textures = textures;

    if(vertices.size() == 0){
        LOG_W(LOGGING_NAME, "Empty vertices array passed in constructor");
    }

    if(indices.size() == 0){
        LOG_W(LOGGING_NAME, "Empty indices array passed in constructor");
    }

    if(textures.size() == 0){
        LOG_W(LOGGING_NAME, "Empty textures array passed in constructor");
    }

    setupMesh();
}

void
Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    // position for vertices, stride = sizeof(Vertex), has to skip to the next vertex struct to find the next positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

    // texture coords (x, y)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

    glBindVertexArray(0);
}

void
Mesh::Draw(GLuint &shaderProgramHandler)
{
    GLuint diffuseNr  = 0;
    GLuint specularNr = 0;

    for(unsigned int i = 0; i < m_textures.size(); i++){
        if(i>=25){
            continue;
        }
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string textureName = m_textures[i].type;
        if(textureName == "texture_diffuse"){
            number = std::to_string(diffuseNr);
            diffuseNr++;
        }
        else if(textureName == "texture_specular"){
            number = std::to_string(specularNr);
            specularNr++;
        }

        std::string uniformMaterialIdx = "material." + textureName + number;
        GLuint uniformId = glGetUniformLocation(shaderProgramHandler, uniformMaterialIdx.c_str());
        glUniform1i(uniformId, i);

        glBindTexture(GL_TEXTURE_2D, m_textures[i].id); 
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,  0);
    glBindVertexArray(0);

}
