#pragma once

#include "ILightComponent.h"
#include <string>

class DirectionalLight : public ILightComponent
{
private:
    static constexpr const char* const LOGGING_NAME = "lights::DirectionalLight";

public:
    // 16 byte alligned
    struct DirectionalLightGPU
    {
        // 16
        glm::vec4 m_lightDirection   = glm::vec4(0);
        
        // 16
        glm::vec4 m_ambientColor     = glm::vec4(0);
        
        // 16
        glm::vec4 m_diffuseColor     = glm::vec4(0);

        // 16
        glm::vec4 m_specularColor    = glm::vec4(0);

        // 16 * 4
        glm::mat4 m_lightVP          = glm::mat4(1); 

        // 16
        float     m_ambientStrength  = 0.0f;
        float     m_specularShininess= 0.0f;
        uint32_t  pad[2];
    };
    static_assert(sizeof(DirectionalLightGPU) % 16 == 0, "UBO structure in Directional Light is not 16 byte alligned");

public:
    DirectionalLight(const std::string& name, uint8_t maxInstances);

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
    
    ~DirectionalLight()override;

private:
    GLuint              m_ubo = 0;
    DirectionalLightGPU m_lightInfo;
    std::string         m_name;
    uint32_t            m_uboOffset = 0;
    bool                m_shadowsEnabled = false;

    glm::vec4           m_position = glm::vec4(0);
    bool                m_positionSet = false;

    GLuint              m_depthMapFbo = 0;
    GLuint              m_depthMapTexture = 0;

public:
    static uint32_t nrOfInstances;
};