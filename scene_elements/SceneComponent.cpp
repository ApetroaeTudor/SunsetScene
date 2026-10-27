#include "SceneComponent.h"
#include <glm/gtx/transform.hpp>

SceneComponent::SceneComponent() :
    m_modelMat(glm::mat4(1))
{
}

void
SceneComponent::Display(const glm::mat4& parentalModelMat)
{
    for(const auto& component : m_sceneComponents){
        component->Display(parentalModelMat * m_modelMat);
    }
}

void
SceneComponent::ScanDepth(const glm::mat4& parentalModelMat, ShaderProgram& depthShaderProgram)
{
    for(const auto& component : m_sceneComponents){
        component->ScanDepth(parentalModelMat * m_modelMat, depthShaderProgram);
    }
}

void
SceneComponent::addChild(const ISceneComponentPtr& childSceneComponent)
{
    if(childSceneComponent == nullptr){
        return; //this should log
    }
    m_sceneComponents.push_back(childSceneComponent);
}


void
SceneComponent::rotate(float angle, const glm::vec3& axes)
{
    // orbit the whole system
    m_modelMat = glm::rotate(angle, axes) * m_modelMat;
}

void
SceneComponent::setNormalsInverted()
{
    // not implemented
    return;
}
