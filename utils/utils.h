#pragma once

#include <glm/ext/scalar_constants.hpp>
#include <string>
#include "../scenes/IScene.h"
#include <GL/glew.h>

constexpr const float PI = glm::pi<float>(); 

std::string ReadFile(const std::string& path);

void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

void initializeStartPoint(int argc, char **argv);
void changeCurrentScene(const IScenePtr& newScene);
void glutDisplayWrapper();
