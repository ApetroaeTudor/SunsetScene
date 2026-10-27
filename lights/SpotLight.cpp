#include "SpotLight.h"
#include <cassert>
#include "ILightComponent.h"
#include "constants.h"
#include "logging.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

uint32_t SpotLight::nrOfInstances = 0;

SpotLight::SpotLight(const std::string& name, uint8_t maxInstances)
    : m_name(name)
{
    nrOfInstances += 1;
    assert(nrOfInstances <= maxInstances);
    m_instanceID = nrOfInstances;
}

SpotLight::~SpotLight()
{
    nrOfInstances -=1;
}

void
SpotLight::setUbo(GLuint ubo)
{
    m_ubo = ubo;
}

void
SpotLight::writeInUbo()
{
    if(m_ubo == 0){
        LOG_E(LOGGING_NAME, m_name + " UBO is not initialized");
        return;
    }
    
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    uint8_t index = m_instanceID - 1;
    uint32_t offset = m_uboOffset + index * sizeof(SpotLight::SpotLightGPU);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(SpotLight::SpotLightGPU), &m_lightInfo);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
SpotLight::setSpecularColor(const glm::vec4& color)
{
    m_lightInfo.m_specularColor = color;
}

void
SpotLight::setSpecularShininess(float shininess)
{
    m_lightInfo.m_specularShininess = shininess;
}

void
SpotLight::setDiffuseColor(const glm::vec4& color)
{
    m_lightInfo.m_diffuseColor = color;
}

void
SpotLight::setAmbientColor(const glm::vec4& color)
{
    m_lightInfo.m_ambientColor = color;
}

void
SpotLight::setAmbientStrength(float strength)
{
    m_lightInfo.m_ambientStrength = strength;
}

void
SpotLight::setLightDirection(const glm::vec4& direction)
{
    m_lightInfo.m_lightDirection = direction;
}

void
SpotLight::setLightPosition(const glm::vec4& position)
{
    m_lightInfo.m_lightPosition = position;
    m_positionSet = true;
}

void
SpotLight::setUboOffset(uint32_t offset)
{
    m_uboOffset = offset;
}

void
SpotLight::setCutoffAngleCos(float cutoffAngleCos)
{
    m_lightInfo.m_cutoffAngleCos = cutoffAngleCos;
}

void
SpotLight::setOuterCutoffAngleCos(float outerCutoffAngleCos)
{
    m_lightInfo.m_outerCutoffAngleCos = outerCutoffAngleCos;
}

void
SpotLight::setAttenuationConstants(const std::array<float, 3>& constants)
{
    LOG_E(LOGGING_NAME, m_name + " No attenuation constants used in Spotlight");
}

void
SpotLight::enableShadows()
{
    if(m_positionSet == false){
        LOG_E(LOGGING_NAME, m_name + " Please first set the light position before activating shadow calculations");
        return;
    }
    m_shadowsEnabled = true;

    glm::mat4 lightProjection = glm::perspective(
      glm::radians<float>(SPOT_LIGHT_SHADOW_CONE_ANGLE * 2.0),
    1.0F,
     SPOT_LIGHT_SHADOW_NEAR_PLANE,
      SPOT_LIGHT_SHADOW_FAR_PLANE
        );
    
    glm::vec3 dir = glm::normalize(glm::vec3(m_lightInfo.m_lightDirection));
    glm::vec3 up = (glm::abs(glm::dot(dir, glm::vec3(0,1,0))) > 0.99f)
                ? glm::vec3(1,0,0)
                : glm::vec3(0,1,0);

    glm::mat4 lightView = glm::lookAt(
        glm::vec3(m_lightInfo.m_lightPosition),
        glm::vec3(m_lightInfo.m_lightPosition) + dir,
        up
    );

    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    m_lightInfo.m_lightVP = lightSpaceMatrix;

    glGenFramebuffers(1, &m_depthMapFbo);
    glGenTextures(1, &m_depthMapTexture);

    glBindTexture(GL_TEXTURE_2D, m_depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WINDOW_SIZE_W, SHADOW_WINDOW_SIZE_H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0 + SPOT_LIGHT_MAP_1 + (m_instanceID-1) );
    glBindTexture(GL_TEXTURE_2D, m_depthMapTexture); 
    glActiveTexture(GL_TEXTURE0);
};

void
SpotLight::disableShadows()
{
    m_shadowsEnabled = false; 
};

bool
SpotLight::shadowsEnabled()
{
    return m_shadowsEnabled;
}

void
SpotLight::activateDepthFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);
}

void
SpotLight::deactivateDepthFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4
SpotLight::getLightVP()
{
    return m_lightInfo.m_lightVP;
}

