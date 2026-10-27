#pragma once

#include "ILightComponent.h"

class PointLight : public ILightComponent
{
private:
    static constexpr const char* const LOGGING_NAME = "lights::PointLight";

public:
    struct PointLightGPU
    {
        // 16
        glm::vec4 m_lightPosition = glm::vec4(0);

        // 16
        glm::vec4 m_ambientColor  = glm::vec4(0);

        // 16
        glm::vec4 m_diffuseColor  = glm::vec4(0);

        // 16
        glm::vec4 m_specularColor = glm::vec4(0);

        // 16
        float m_ambientStrength;
        float m_specularShininess;
        float m_KC;
        float m_KL;

        // 16
        float m_KQ;
        uint32_t padding[3];
    };
    static_assert(sizeof(PointLightGPU) % 16 == 0, "UBO structure in Point Light is not 16 byte alligned");

public:
    PointLight(const std::string& name, uint8_t maxInstances);

    void setUbo(GLuint ubo)override;

    void writeInUbo()override;
    
    void setSpecularColor(const glm::vec4& color)override;
    void setSpecularShininess(float shininess)override;
    
    void setDiffuseColor(const glm::vec4& color)override;
    
    void setAmbientColor(const glm::vec4& color)override;
    void setAmbientStrength(float strength)override;

    void setLightDirection(const glm::vec4& direction)override;

    void setLightPosition(const glm::vec4& position)override;

    void setUboOffset(uint32_t offset)override;

    void setCutoffAngleCos(float cutoffAngleCos)override;
    void setOuterCutoffAngleCos(float outerCutoffAngleCos)override;

    void setAttenuationConstants(const std::array<float, 3>& constants)override;

    void enableShadows()override;
    void disableShadows()override;
    bool shadowsEnabled()override;

    void activateDepthFramebuffer()override;
    void deactivateDepthFramebuffer()override;
    glm::mat4 getLightVP()override;
    
    ~PointLight()override;

private:
    GLuint        m_ubo = 0;
    PointLightGPU m_lightInfo;
    std::string   m_name;
    uint32_t      m_uboOffset  = 0;
    uint32_t      m_instanceID = 0;
    bool          m_shadowsEnabled = false;

public:
    static uint32_t nrOfInstances;

};