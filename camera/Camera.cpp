#include "Camera.h"
#include "room_scene/RoomScene.h"
#include <GL/freeglut_std.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <chrono>

static double lastFrameTime    = 0.0f;
static double currentFrameTime = 0.0f;

static int lastX;
static int lastY;
static bool firstTime = true;

static float yaw   = -90.0f;
static float pitch = 0.0f;

glm::vec3 cameraPos          = glm::vec3(0.0f, CAMERA_HEIGHT,  3.0);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

static void KeyboardCallback(unsigned char key, int x, int y);
static void MouseCallback(int x, int y);

glm::mat4 viewMatrix = glm::mat4(1);

Camera::Camera()
{}

void
Camera::initializeCamera()
{
    glutKeyboardUpFunc(KeyboardCallback);
    glutPassiveMotionFunc(MouseCallback);
    glutMotionFunc(MouseCallback);

    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

static void
KeyboardCallback(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'W':
        case 'w': 
        {
            cameraPos += MOVING_SPEED * cameraFront;
            break;
        }
        case 'A':
        case 'a': 
        {
            cameraPos -= MOVING_SPEED * glm::normalize(glm::cross(cameraFront, cameraUp));
            break; 
        }
        case 'S':
        case 's':
        {
            cameraPos -= MOVING_SPEED * cameraFront;
            break;
        }
        case 'D':
        case 'd':
        {
            cameraPos += MOVING_SPEED * glm::normalize(glm::cross(cameraFront, cameraUp));
            break;
        }
    }

    cameraPos.y = CAMERA_HEIGHT;

    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glutPostRedisplay();
}

static void
MouseCallback(int x, int y)
{
    if(firstTime){
        lastX = x;
        lastY = y;
        firstTime = false;
        return;
    }

    double deltaX = (x - lastX) * CAMERA_SENSITIVITY;
    double deltaY = (lastY - y) * CAMERA_SENSITIVITY;

    yaw += deltaX;
    pitch += deltaY;

    lastX = x;
    lastY = y;

    pitch = std::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glutPostRedisplay();
}