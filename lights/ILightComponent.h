#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <memory>

class ILightComponent
{
public:
    virtual void setUbo(GLuint ubo) = 0;
    virtual void writeInUbo()      = 0;

    virtual void setSpecularColor(const glm::vec4& color) = 0;
    virtual void setSpecularShininess(float shininess)    = 0;

    virtual void setDiffuseColor(const glm::vec4& color)  = 0;

    virtual void setAmbientColor(const glm::vec4& color)  = 0;
    virtual void setAmbientStrength(float strength)       = 0;
    
    virtual void setLightDirection(const glm::vec4& direction) = 0;
    virtual void setLightPosition(const glm::vec4& position)   = 0;

    virtual void setCutoffAngleCos(float cutoffAngleCos) = 0;
    virtual void setOuterCutoffAngleCos(float outerCutoffAngleCos) = 0;

    virtual void setAttenuationConstants(const std::array<float, 3>& constants) = 0;

    virtual void setUboOffset(uint32_t offset) = 0;

    virtual void enableShadows()  = 0;
    virtual void disableShadows() = 0;

    virtual void activateDepthFramebuffer() = 0;
    virtual void deactivateDepthFramebuffer() = 0;
    virtual bool shadowsEnabled() = 0;
    virtual glm::mat4 getLightVP() = 0;

    virtual ~ILightComponent() = default;
};

typedef std::shared_ptr<ILightComponent> ILightComponentPtr;