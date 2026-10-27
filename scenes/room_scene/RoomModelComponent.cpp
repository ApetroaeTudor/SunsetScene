#include "RoomModelComponent.h"
#include "ISceneComponent.h"
#include "utils.h"
#include "constants.h"
#include "shaders/ShaderProgram.h"

#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include "logging.h"

extern glm::mat4 projectionMatrix;
extern glm::mat4 viewMatrix;
extern glm::vec3 cameraPos;

RoomModelComponent::RoomModelComponent(const glm::mat4& scaleMatrix, const std::string& modelPath, const std::string& name)
    : m_scaleMatrix(scaleMatrix)
    , m_tableModel(name + "-Model")
    , m_shaderProgram(name + "-Program")
    , m_name(name)
    , m_modelPath(modelPath)
{
    setupGLStuff();
}

void
RoomModelComponent::setupGLStuff()
{
    m_tableModel.loadModel(m_modelPath);
    
    if(!m_shaderProgram.initializeProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)){
        LOG_E(LOGGING_NAME, "Shader program init failed");
        return;
    }
    ShaderProgram::setDefaultUniforms(m_shaderProgram);
    
    m_isValid = true;
}

void
RoomModelComponent::Display(const glm::mat4& parentalModelMat)
{
    m_shaderProgram.use();
    glm::mat4 rawModel = parentalModelMat * m_modelMatrix * m_scaleMatrix;
    glm::mat4 finalModel = projectionMatrix * viewMatrix * rawModel;
    m_shaderProgram.setMat4("modelViewProjectionMatrix", finalModel);
    m_shaderProgram.setMat4("model", rawModel);
    m_shaderProgram.setVec3("cameraPos", cameraPos);

    if(m_normalsInverted){
        m_shaderProgram.setInt("normalsInverted", 1);
        m_tableModel.Draw(m_shaderProgram.shaderId());
        m_shaderProgram.setInt("normalsInverted", 0);
    }
    else{
        m_tableModel.Draw(m_shaderProgram.shaderId());
    }
}

void
RoomModelComponent::ScanDepth(const glm::mat4& parentalModelMat, ShaderProgram& depthShaderProgram)
{
    depthShaderProgram.setMat4("model", parentalModelMat * m_modelMatrix * m_scaleMatrix);
    m_tableModel.Draw(depthShaderProgram.shaderId());
}

void
RoomModelComponent::addChild(const ISceneComponentPtr& childSceneComponent)
{
    // leftover from Composite pattern, not implemented in leaf component
    return;
}

bool
RoomModelComponent::isValid()
{
    return m_isValid;
}

void
RoomModelComponent::updateModelMat(const glm::mat4& modelMat)
{
    m_modelMatrix = m_modelMatrix * modelMat;
}

glm::mat4
RoomModelComponent::getModelMat(const glm::mat4& modelMat)
{
    return m_modelMatrix;
}

void
RoomModelComponent::rotate(float angle, const glm::vec3& axes)
{
    // multiply to right because:
    // the rotation is applied to the local coordinate system
    // if it were applied to the left, then the rotation would look like an orbit
    m_modelMatrix = m_modelMatrix * glm::rotate(angle, axes);
}
    
void
RoomModelComponent::setNormalsInverted()
{
    m_normalsInverted = true;
}