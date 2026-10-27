#pragma once

#include "scene_elements/ISceneComponent.h"
#include "assimp_bridge/Model.h"
#include "shaders/ShaderProgram.h"
#include <glm/ext/matrix_float4x4.hpp>

class RoomModelComponent: public ISceneComponent
{
private:
    static constexpr const char* const LOGGING_NAME = "room_scene::TableComponent";

public:
    RoomModelComponent(const glm::mat4& scaleMatrix, const std::string& modelPath, const std::string& name = "");

    void Display(const glm::mat4& parentalModelMat)override;

    void ScanDepth(const glm::mat4& parentalModelMat, ShaderProgram& depthShaderProgram)override;
    
    void addChild(const ISceneComponentPtr& childSceneComponent)override;

    bool isValid()override;

    void updateModelMat(const glm::mat4& modelMat)override;

    glm::mat4 getModelMat(const glm::mat4& modelMat)override;

    void rotate(float angle, const glm::vec3& axes)override;

    void setNormalsInverted()override;
    
    ~RoomModelComponent()override = default;

private:
    void setupGLStuff();

private:
    ShaderProgram m_shaderProgram;
    Model         m_tableModel;
    glm::mat4     m_modelMatrix = glm::mat4(1);
    glm::mat4     m_scaleMatrix = glm::mat4(1);
    std::string   m_modelPath;
    std::string   m_name;
    bool          m_normalsInverted = false;

    bool m_isValid = false;
};