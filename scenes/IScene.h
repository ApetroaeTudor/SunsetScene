#pragma once

#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <stdint.h>
#include <memory>

class IScene
{
public:
    virtual void displayScene()         = 0;
    virtual void initializeScene()      = 0;
    virtual void startUpdate(int value) = 0;
    virtual void activateScene()        = 0;
    virtual void initializeLightingUbo()= 0;
    virtual void initializeLights()     = 0;

    virtual ~IScene() = default;
};

using IScenePtr = std::shared_ptr<IScene>;