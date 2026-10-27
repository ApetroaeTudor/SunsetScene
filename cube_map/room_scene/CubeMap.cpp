#include "CubeMap.h"
#include "Camera.h"
#include "logging.h"
#include "shaders/ShaderProgram.h"
#include "stb/stb_image.h"
#include "constants.h"

extern glm::mat4 viewMatrix;
extern glm::mat4 projectionMatrix;

CubeMap::CubeMap(const std::string& name)
    : m_name(name)
    , m_shaderProgram(name + " ShaderProgram")
{
}

bool
CubeMap::loadCubeMap()
{
    if(!m_shaderProgram.initializeProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)){
        LOG_E(LOGGING_NAME, m_name + " Failed to initialize shader program");
    }
    ShaderProgram::setDefaultUniforms(m_shaderProgram);
    
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

    int width;
    int height;
    int nrChannels;

    for(int i = 0; i < texture_paths.size(); i++){
        auto data = stbi_load(texture_paths[i].data(), &width, &height, &nrChannels,0);
        if(data == nullptr){
            LOG_E(LOGGING_NAME, m_name + " Failed to load cube map image");
            stbi_image_free(data);
            return false;
        }

        GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : (nrChannels == 4) ? GL_RGBA : -1;
        if(format == -1){
            LOG_E(LOGGING_NAME, m_name + " Unrecognized format, nr of channels in image is off");
            return false;
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, 
            format, 
            width, 
            height, 
            0, 
            format, 
            GL_UNSIGNED_BYTE, 
            data
        );
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glStuff();

    return true;
}

void
CubeMap::Display()
{
    m_shaderProgram.use();
    m_shaderProgram.setInt("isDrawingSkybox", 1);
    m_shaderProgram.setInt("cubemap", 1);
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

    glm::mat4 viewNoTx = glm::mat4(glm::mat3(viewMatrix));
    m_shaderProgram.setMat4("modelViewProjectionMatrix", projectionMatrix * viewNoTx);
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(m_vao);
    glDepthMask(GL_FALSE);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
    m_shaderProgram.setInt("isDrawingSkybox", 0);
    glActiveTexture(GL_TEXTURE0);
    glDepthMask(GL_TRUE);
}

void
CubeMap::glStuff()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // texture coords (x, y)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
}