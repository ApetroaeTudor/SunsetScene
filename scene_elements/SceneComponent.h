#pragma once

#include "ISceneComponent.h"
#include "shaders/ShaderProgram.h"
#include <vector>

class SceneComponent: public ISceneComponent
{
public:

    SceneComponent();

    void Display(const glm::mat4& parentalModelMat)override;

    void ScanDepth(const glm::mat4& parentalModelMat, ShaderProgram& depthShaderProgram)override;
    
    bool isValid()override { return !m_sceneComponents.empty(); };
    
    void addChild(const ISceneComponentPtr& childSceneComponent)override;

    void updateModelMat(const glm::mat4& modelMat)override { m_modelMat = modelMat * m_modelMat; }

    glm::mat4 getModelMat(const glm::mat4& modelMat)override { return m_modelMat; }

    void rotate(float angle, const glm::vec3& axes)override;
    
    void setNormalsInverted()override;

    ~SceneComponent()override = default;

private:
    std::vector<ISceneComponentPtr> m_sceneComponents;    
    glm::mat4 m_modelMat;
};

using SceneComponentPtr = std::shared_ptr<SceneComponent>;