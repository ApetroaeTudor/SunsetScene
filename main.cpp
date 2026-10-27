#include "room_scene/RoomScene.h"
#include "utils.h"
#include "memory"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include "logging.h"

GLuint shaderProgram;

int main(int argc, char **argv){
    initializeStartPoint(argc, argv);
    initLogger(LOG_FILE);

    auto robotArmScene = std::make_shared<RoomScene>();
    robotArmScene->activateScene();

    glutMainLoop();

    return 0;
}
