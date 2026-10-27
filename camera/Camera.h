#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"

class Camera
{
public:
    Camera();
    ~Camera() = default;
    void initializeCamera();
};