#include "PointLight.h"
#include "logging.h"
#include <array>

uint32_t PointLight::nrOfInstances = 0;

PointLight::PointLight(const std::string& name, uint8_t maxInstances)
    : m_name(name)
{ 
    nrOfInstances += 1;
    assert(nrOfInstances <= maxInstances);
    m_instanceID = nrOfInstances;
}

PointLight::~PointLight()
{
    nrOfInstances -= 1;    
}

void
PointLight::setUbo(GLuint ubo)
{
    m_ubo = ubo;
}

void
PointLight::writeInUbo()
{
    if(m_ubo == 0){
        LOG_E(LOGGING_NAME, m_name + " UBO is not initialized");
        return;
    }
    
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    uint8_t index = m_instanceID - 1;
    uint32_t offset = m_uboOffset + index * sizeof(PointLight::PointLightGPU);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(PointLight::PointLightGPU), &m_lightInfo);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
    
void
PointLight::setSpecularColor(const glm::vec4& color)
{
    m_lightInfo.m_specularColor = color;
}

void
PointLight::setSpecularShininess(float shininess)
{
    m_lightInfo.m_specularShininess = shininess;
}
    
void
PointLight::setDiffuseColor(const glm::vec4& color)
{
    m_lightInfo.m_diffuseColor = color;
}
    
void
PointLight::setAmbientColor(const glm::vec4& color)
{
    m_lightInfo.m_ambientColor = color;
}
    
void
PointLight::setAmbientStrength(float strength)
{
    m_lightInfo.m_ambientStrength = strength;
}

void
PointLight::setLightDirection(const glm::vec4& direction)
{
    LOG_E(LOGGING_NAME, m_name + " No light direction in point light");
}

void
PointLight::setLightPosition(const glm::vec4& position)
{
    m_lightInfo.m_lightPosition = position;
}

void
PointLight::setUboOffset(uint32_t offset)
{
    m_uboOffset = offset;
}

void
PointLight::setCutoffAngleCos(float cutoffAngleCos)
{
    LOG_E(LOGGING_NAME, m_name + " No light cutoff angle in point light");
}

void
PointLight::setOuterCutoffAngleCos(float outerCutoffAngleCos)
{
    LOG_E(LOGGING_NAME, m_name + " No light outer cutoff angle in point light");
}

void
PointLight::setAttenuationConstants(const std::array<float, 3>& constants)
{
    m_lightInfo.m_KC = constants.at(0);
    m_lightInfo.m_KL = constants.at(1);
    m_lightInfo.m_KQ = constants.at(2);
}

void
PointLight::activateDepthFramebuffer()
{

}

glm::mat4
PointLight::getLightVP()
{
    return {};
}

void
PointLight::deactivateDepthFramebuffer()
{
    return;
}

void
PointLight::enableShadows()
{ 
    m_shadowsEnabled = true;
};

void
PointLight::disableShadows()
{
    m_shadowsEnabled = false;
};

bool
PointLight::shadowsEnabled()
{
    return m_shadowsEnabled;
}