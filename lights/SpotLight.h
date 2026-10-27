#pragma once

#include "ILightComponent.h"

class SpotLight : public ILightComponent
{
private:
    static constexpr const char* const LOGGING_NAME = "lights::SpotLight";

public:
    struct SpotLightGPU
    {
        // 16
        glm::vec4 m_lightDirection = glm::vec4(0);

        // 16
        glm::vec4 m_lightPosition  = glm::vec4(0);

        // 16
        glm::vec4 m_diffuseColor   = glm::vec4(0);

        // 16
        glm::vec4 m_specularColor  = glm::vec4(0);

        // 16
        glm::vec4 m_ambientColor   = glm::vec4(0);

        // 16 * 4
        glm::mat4 m_lightVP        = glm::mat4(0);

        // 16
        float m_specularShininess;
        float m_cutoffAngleCos;
        float m_outerCutoffAngleCos;
        float m_ambientStrength;
    };
    static_assert(sizeof(SpotLightGPU) % 16 == 0, "UBO structure in Spot Light is not 16 byte alligned");

public:
    SpotLight(const std::string& name, uint8_t maxInstances);

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

    ~SpotLight()override;

private:
    GLuint       m_ubo = 0;
    SpotLightGPU m_lightInfo;
    std::string  m_name;
    uint32_t     m_uboOffset  = 0;
    uint32_t     m_instanceID = 0;
    bool         m_shadowsEnabled = false;
    bool         m_positionSet = false;

    GLuint              m_depthMapFbo = 0;
    GLuint              m_depthMapTexture = 0;

public:
    static uint32_t nrOfInstances;

};