#include "utils.h"
#include <iostream>
#include <spdlog/common.h>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include "constants.h"

glm::mat4 projectionMatrix;
IScenePtr globalScene;

std::string
ReadFile(const std::string& path)
{
    std::string resultData;
    if(path.empty()){
        return resultData;
    }

    std::ifstream file(path);
    if(!file.is_open()){
        return resultData;
    }

    std::string line;
    while(std::getline(file,line)){
        resultData += line + '\n';
    }

    file.close();

    return resultData;
}

void
reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    projectionMatrix = glm::perspective(glm::radians<float>(75.0), (float)w / h, 0.1f, 100.0f);
}

void
initializeStartPoint(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(INITIAL_WINDOW_POSITION_X, INITIAL_WINDOW_POSITION_Y);
    glutInitWindowSize(INITIAL_WINDOW_SIZE_W, INITIAL_WINDOW_SIZE_H);
    glutCreateWindow(WINDOW_NAME);
    
    glewInit();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    glutReshapeFunc(reshape);
}
