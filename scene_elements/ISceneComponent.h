#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include <GL/glew.h>
#include "memory"
#include "shaders/ShaderProgram.h"

class ISceneComponent;
using ISceneComponentPtr = std::shared_ptr<ISceneComponent>;

class ISceneComponent
{
public:
    virtual void Display(const glm::mat4& parentalModelMat) = 0;

    virtual void ScanDepth(const glm::mat4& parentalModelMat, ShaderProgram& depthShaderProgram) = 0;
    
    virtual void addChild(const ISceneComponentPtr& childSceneComponent) = 0;

    virtual bool isValid() = 0;

    virtual void updateModelMat(const glm::mat4& modelMat) = 0;

    virtual glm::mat4 getModelMat(const glm::mat4& modelMat) = 0;

    virtual void rotate(float angle, const glm::vec3& axes) = 0;

    virtual void setNormalsInverted() = 0;
    
    virtual ~ISceneComponent() = default;
};