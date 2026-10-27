#include "DirectionalLight.h"

#include "Camera.h"
#include "ILightComponent.h"
#include "IScene.h"
#include "constants.h"
#include "logging.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <memory>

uint32_t DirectionalLight::nrOfInstances = 0;

DirectionalLight::DirectionalLight(const std::string& name, uint8_t maxInstances)
    : m_name(name)
{
    nrOfInstances +=1;
    assert(nrOfInstances <= maxInstances);
}

DirectionalLight::~DirectionalLight()
{
    nrOfInstances -=1;
}

void
DirectionalLight::setUbo(GLuint ubo)
{
    m_ubo = ubo;
}

void
DirectionalLight::writeInUbo()
{
    if(m_ubo == 0){
        LOG_E(LOGGING_NAME, m_name + " UBO is not initialized");
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    uint8_t index = DirectionalLight::nrOfInstances - 1;
    glBufferSubData(GL_UNIFORM_BUFFER, m_uboOffset, sizeof(DirectionalLight::DirectionalLightGPU), &m_lightInfo);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
DirectionalLight::setSpecularColor(const glm::vec4& color)
{
    m_lightInfo.m_specularColor = color;
}

void
DirectionalLight::setSpecularShininess(float shininess)
{
    m_lightInfo.m_specularShininess = shininess;
}

void
DirectionalLight::setDiffuseColor(const glm::vec4& color)
{
    m_lightInfo.m_diffuseColor = color;
}

void
DirectionalLight::setAmbientColor(const glm::vec4& color)
{
    m_lightInfo.m_ambientColor = color;
}

void
DirectionalLight::setAmbientStrength(float strength)
{
    m_lightInfo.m_ambientStrength = strength;
}

void
DirectionalLight::setLightDirection(const glm::vec4& direction)
{
    m_lightInfo.m_lightDirection = direction;
}

void
DirectionalLight::setLightPosition(const glm::vec4& position)
{
    m_position = position;
    m_positionSet = true;
}

void
DirectionalLight::setUboOffset(uint32_t offset)
{
    m_uboOffset = offset;
}

void
DirectionalLight::setCutoffAngleCos(float cutoffAngleCos)
{
    LOG_E(LOGGING_NAME, m_name + " Directional light has no cutoff angle");
}

void
DirectionalLight::setOuterCutoffAngleCos(float outerCutoffAngleCos)
{
    LOG_E(LOGGING_NAME, m_name + " Directional light has no outer cutoff angle");
}

void
DirectionalLight::setAttenuationConstants(const std::array<float, 3>& constants)
{
    LOG_E(LOGGING_NAME, m_name + " No attenuation constants used in Directional Light");
}

void
DirectionalLight::enableShadows()
{
    if(m_positionSet == false){
        LOG_E(LOGGING_NAME, m_name + " Please first set the light position before activating shadow calculations");
        return;
    }
    m_shadowsEnabled = true;
    
    glm::mat4 lightProjection = glm::ortho(
        -DIRECTIONAL_LIGHT_SHADOW_FRUSTUM_SZ,
        DIRECTIONAL_LIGHT_SHADOW_FRUSTUM_SZ,
      -DIRECTIONAL_LIGHT_SHADOW_FRUSTUM_SZ,
          DIRECTIONAL_LIGHT_SHADOW_FRUSTUM_SZ,
        DIRECTIONAL_LIGHT_SHADOW_NEAR_PLANE,
         DIRECTIONAL_LIGHT_SHADOW_FAR_PLANE
    );
    
    glm::mat4 lightView = glm::lookAt(glm::vec3(m_position), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F));

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

    glActiveTexture(GL_TEXTURE0 + DIRECTIONAL_LIGHT_MAP_1);
    glBindTexture(GL_TEXTURE_2D, m_depthMapTexture); 
    glActiveTexture(GL_TEXTURE0);
}

void
DirectionalLight::activateDepthFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo);
}

void
DirectionalLight::disableShadows()
{
    m_shadowsEnabled = false; 
}

bool
DirectionalLight::shadowsEnabled()
{
    return m_shadowsEnabled;
}

glm::mat4
DirectionalLight::getLightVP()
{
    return m_lightInfo.m_lightVP;
}

void
DirectionalLight::deactivateDepthFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
