#include "RoomScene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "ILightComponent.h"
#include "ISceneComponent.h"
#include "PointLight.h"
#include "SceneComponent.h"
#include "SpotLight.h"
#include "colors.h"
#include "constants.h"
#include "room_scene/RoomModelComponent.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cstddef>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include "logging.h"
#include "utils.h"

extern IScenePtr globalScene;

static void RoomSceneDisplayWrapper();

RoomScene::RoomScene()
    : m_cubeMap("RoomScene CubeMap")
    , m_depthTestingShader("DepthTestingShaderProgram")
{
    initializeScene();
}

void
RoomScene::displayScene()
{
    if(m_topLevelComponent == nullptr){
        LOG_E(LOGGING_NAME, "Can't draw a scene with no top level component");
        return;
    }
    // peter panning
    glCullFace(GL_FRONT);
    // do depth testing first
    glViewport(0, 0, SHADOW_WINDOW_SIZE_W, SHADOW_WINDOW_SIZE_H);
    m_depthTestingShader.use();

    for(const auto& lightSource : m_lightComponents){
        if(lightSource->shadowsEnabled() == false){
            continue;
        }
        lightSource->activateDepthFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);
        m_depthTestingShader.setMat4("lightSpaceMatrix", lightSource->getLightVP());
        m_topLevelComponent->ScanDepth(glm::mat4(1), m_depthTestingShader);
        lightSource->deactivateDepthFramebuffer();
    }
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    
    m_cubeMap.Display();

    for(const auto& lightSource : m_lightComponents){
        lightSource->writeInUbo();
    }

    m_topLevelComponent->Display(glm::mat4(1));
    glutSwapBuffers();
}

void
RoomScene::initializeScene()
{
    initializeLights();
    m_depthTestingShader.initializeProgram(DEPTH_VERTEX_SHADER_PATH, DEPTH_FRAGMENT_SHADER_PATH);
    m_camera.initializeCamera();
    m_cubeMap.loadCubeMap();

    m_topLevelComponent = std::make_shared<SceneComponent>();
    auto neutralScaleMat = glm::scale(glm::mat4(1), glm::vec3(1,1,1));
    auto ninetyDegreeRotation = glm::rotate(glm::mat4(1), PI/2, glm::vec3(1,0,0));


    auto floorScaleMat = glm::scale(glm::mat4(1), glm::vec3(FLOOR_SCALE, FLOOR_SCALE, FLOOR_SCALE));
    ISceneComponentPtr floor = std::make_shared<RoomModelComponent>(floorScaleMat, FLOOR_MODEL_PATH, "Floor");
    floor->updateModelMat(ninetyDegreeRotation);
    floor->setNormalsInverted();

    // TABLE SYSTEM ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    ISceneComponentPtr tableSystem = std::make_shared<SceneComponent>();
    ISceneComponentPtr pointlightLampSubSystem = std::make_shared<SceneComponent>();

    auto tableSystemTxMat = glm::translate(glm::mat4(1), TABLE_POS);
    auto pointlightLampSubsystemTxMat = glm::translate(glm::mat4(1), LAMP_POINTLIGHT_POS);

    auto tableScaleMat = glm::scale(glm::mat4(1), glm::vec3(TABLE_SCALE,TABLE_SCALE, TABLE_SCALE)); 
    ISceneComponentPtr table = std::make_shared<RoomModelComponent>(tableScaleMat, TABLE_MODEL_PATH, "Table");

    auto lampPointLightScaleMat = glm::scale(glm::mat4(1), glm::vec3(LAMP_POINTLIGHT_SCALE, LAMP_POINTLIGHT_SCALE, LAMP_POINTLIGHT_SCALE)); 
    ISceneComponentPtr lampPointLight = std::make_shared<RoomModelComponent>(lampPointLightScaleMat, LAMP_POINTLIGHT_MODEL_PATH, "LampPointLight");
    auto lampPointLightModel = glm::rotate(glm::mat4(1), glm::radians(LAMP_POINTLIGHT_ROTATION_ANGLE), LAMP_POINTLIGHT_ROTATION_DIRECTION)* lampPointLightScaleMat;
    lampPointLight->updateModelMat(lampPointLightModel);

    tableSystem->addChild(table);
    tableSystem->addChild(pointlightLampSubSystem);

    pointlightLampSubSystem->addChild(lampPointLight);
    pointlightLampSubSystem->updateModelMat(pointlightLampSubsystemTxMat);
    
    tableSystem->updateModelMat(tableSystemTxMat);
    // ~ TABLE SYSTEM ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    


    auto carpetScaleMat = glm::scale(glm::mat4(1), glm::vec3(CARPET_SCALE, CARPET_SCALE, CARPET_SCALE));
    ISceneComponentPtr carpet = std::make_shared<RoomModelComponent>(carpetScaleMat, CARPET_MODEL_PATH, "Carpet");
    carpet->updateModelMat(ninetyDegreeRotation);
    carpet->setNormalsInverted();

    // LAMP SPOTLIGHT 1 SYSTEM -----------------------------------------------------------------------------------------------------------------------------------------------------------
    ISceneComponentPtr lamp1SpotlightSystem = std::make_shared<SceneComponent>();
    ISceneComponentPtr tableSubsystem = std::make_shared<SceneComponent>();
    
    auto table2ScaleMat = glm::scale(glm::mat4(1), glm::vec3(TABLE_SCALE,TABLE_SCALE, TABLE_SCALE));
    ISceneComponentPtr table2 = std::make_shared<RoomModelComponent>(table2ScaleMat, TABLE_MODEL_PATH, "Table2");
    auto table2TxMat  = glm::translate(glm::mat4(1), TABLE2_POS);
    auto table2RotMat = glm::rotate(glm::mat4(1), glm::radians<float>(TABLE2_ROT_ANGLE), TABLE2_ROT_AXES); 
    table2->updateModelMat(table2TxMat * table2RotMat);

    auto lampSpotlightScaleMat = glm::scale(glm::mat4(1), glm::vec3(SPOTLIGHT_LAMP_SCALE,SPOTLIGHT_LAMP_SCALE, SPOTLIGHT_LAMP_SCALE)); 
    ISceneComponentPtr lampSpotlight = std::make_shared<RoomModelComponent>(lampSpotlightScaleMat, LAMP_SPOTLIGHT_MODEL_PATH, "LampSpotlight");

    lamp1SpotlightSystem->addChild(lampSpotlight);
    lamp1SpotlightSystem->addChild(tableSubsystem);
    tableSubsystem->addChild(table2);
    lamp1SpotlightSystem->updateModelMat(glm::translate(glm::mat4(1), glm::vec3(SPOTLIGHT_LAMP_X, SPOTLIGHT_LAMP_Y , SPOTLIGHT_LAMP_Z)));
    // ~ LAMP SPOTLIGHT 1 SYSTEM -----------------------------------------------------------------------------------------------------------------------------------------------------------

    ISceneComponentPtr lampSpotlight2 = std::make_shared<RoomModelComponent>(lampSpotlightScaleMat, LAMP_SPOTLIGHT_MODEL_PATH, "LampSpotlight");
    lampSpotlight2->updateModelMat(glm::translate(glm::mat4(1), glm::vec3(SPOTLIGHT_LAMP_X, SPOTLIGHT_LAMP_Y , -SPOTLIGHT_LAMP_Z)));

    auto ceilingScaleMat = glm::scale(glm::mat4(1), glm::vec3(1,1,1));
    ISceneComponentPtr ceiling = std::make_shared<RoomModelComponent>(ceilingScaleMat, CEILING_MODEL_PATH, "Ceiling");

    auto basicWallScaleMat = glm::scale(glm::mat4(1), glm::vec3(1,1,1));
    ISceneComponentPtr basicWall = std::make_shared<RoomModelComponent>(basicWallScaleMat, BASIC_WALL_MODEL_PATH, "BasicWall");

    auto wallWindowScaleMat = glm::scale(glm::mat4(1), glm::vec3(1,1,1));
    ISceneComponentPtr wallWindow = std::make_shared<RoomModelComponent>(wallWindowScaleMat, WALL_WINDOW_MODEL_PATH, "WallWindow");

    m_topLevelComponent->addChild(floor);
    m_topLevelComponent->addChild(tableSystem);
    m_topLevelComponent->addChild(carpet);
    m_topLevelComponent->addChild(lamp1SpotlightSystem);
    m_topLevelComponent->addChild(lampSpotlight2);
}
    
void
RoomScene::startUpdate(int value)
{
    return;
}
    
void 
RoomScene::activateScene()
{
    globalScene = shared_from_this();
    glutDisplayFunc(RoomSceneDisplayWrapper);
}

void
RoomScene::initializeLightingUbo()
{
    glGenBuffers(1, &m_lightingUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightingUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightingUbo), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_lightingUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
RoomScene::initializeLights()
{
    initializeLightingUbo();
    ILightComponentPtr directionalLight = std::make_shared<DirectionalLight>("DirectionalLight", ROOM_SCENE_MAX_DIRECTIONAL_LIGHTS);
    directionalLight->setUbo(m_lightingUbo);
    directionalLight->setUboOffset(0);
    directionalLight->setAmbientColor(SUNSET_COLOR);
    directionalLight->setAmbientStrength(DIRECTIONAL_LIGHT_AMBIENT_STRENGTH);
    directionalLight->setDiffuseColor(DARK_BLUE_TINT_PURPLE);
    directionalLight->setSpecularColor(WHITE);
    directionalLight->setSpecularShininess(DIRECTIONAL_LIGHT_SPEC_SHININESS);
    directionalLight->setLightPosition(glm::vec4(DIRECTIONAL_LIGHT_POS, 1.0));
    glm::vec4 dirLightVec = glm::vec4(DIRECTIONAL_LIGHT_X, DIRECTIONAL_LIGHT_Y, DIRECTIONAL_LIGHT_Z, DIRECTIONAL_LIGHT_W);
    directionalLight->setLightDirection(glm::normalize(dirLightVec));
    directionalLight->enableShadows();
    incrementNrOfDirectionalLights();
    m_lightComponents.push_back(directionalLight);

    ILightComponentPtr spotlight1 = std::make_shared<SpotLight>("SpotLight1", ROOM_SCENE_MAX_SPOTLIGHTS);
    spotlight1->setUbo(m_lightingUbo);
    spotlight1->setUboOffset(offsetof(LightingUbo, spotLights));
    spotlight1->setLightDirection(glm::vec4(LAMP1_SPOT_LIGHT_DIR_X, LAMP1_SPOT_LIGHT_DIR_Y, LAMP1_SPOT_LIGHT_DIR_Z, 0.0f));
    spotlight1->setLightPosition(glm::vec4(LAMP1_SPOT_LIGHT_POS_X, LAMP1_SPOT_LIGHT_POS_Y, LAMP1_SPOT_LIGHT_POS_Z, 1.0f));
    spotlight1->setDiffuseColor(WARM_WHITE);
    spotlight1->setSpecularColor(WHITE);
    spotlight1->setSpecularShininess(LAMP1_SPOT_LIGHT_SPEC_SNININESS);
    spotlight1->setCutoffAngleCos(glm::cos(glm::radians(LAMP1_SPOT_LIGHT_CUTOFF_ANGLE)));
    spotlight1->setOuterCutoffAngleCos(glm::cos(glm::radians(LAMP1_SPOT_LIGHT_OUTER_CUTOFF_ANGLE)));
    spotlight1->setAmbientColor(WARM_WHITE);
    spotlight1->setAmbientStrength(LAMP1_AMBIENT_LIGHT_STRENGTH);
    spotlight1->enableShadows();
    incrementNrOfSpotLights();
    m_lightComponents.push_back(spotlight1);

    ILightComponentPtr spotlight2 = std::make_shared<SpotLight>("SpotLight2", ROOM_SCENE_MAX_SPOTLIGHTS);
    spotlight2->setUbo(m_lightingUbo);
    spotlight2->setUboOffset(offsetof(LightingUbo, spotLights));
    spotlight2->setLightDirection(glm::vec4(LAMP1_SPOT_LIGHT_DIR_X, LAMP1_SPOT_LIGHT_DIR_Y, LAMP1_SPOT_LIGHT_DIR_Z, 0.0f));
    spotlight2->setLightPosition(glm::vec4(LAMP1_SPOT_LIGHT_POS_X, LAMP1_SPOT_LIGHT_POS_Y, -LAMP1_SPOT_LIGHT_POS_Z, 1.0f));
    spotlight2->setDiffuseColor(WARM_WHITE);
    spotlight2->setSpecularColor(WHITE);
    spotlight2->setSpecularShininess(LAMP1_SPOT_LIGHT_SPEC_SNININESS);
    spotlight2->setCutoffAngleCos(glm::cos(glm::radians(LAMP1_SPOT_LIGHT_CUTOFF_ANGLE/2.0)));
    spotlight2->setOuterCutoffAngleCos(glm::cos(glm::radians(LAMP1_SPOT_LIGHT_OUTER_CUTOFF_ANGLE/2.0)));
    spotlight2->setAmbientColor(WARM_WHITE);
    spotlight2->setAmbientStrength(LAMP1_AMBIENT_LIGHT_STRENGTH);
    spotlight2->enableShadows();
    incrementNrOfSpotLights();
    m_lightComponents.push_back(spotlight2);

    ILightComponentPtr pointlight1 = std::make_shared<PointLight>("PointLight1", ROOM_SCENE_MAX_POINTLIGHTS);
    pointlight1->setUbo(m_lightingUbo);
    pointlight1->setUboOffset(offsetof(LightingUbo, pointLights));
    pointlight1->setLightPosition(glm::vec4(POINT_LIGHT1_POS, 1.0f));
    pointlight1->setAmbientColor(WARM_WHITE);
    pointlight1->setDiffuseColor(VIVID_YELLOW);
    pointlight1->setSpecularColor(WHITE);
    pointlight1->setAmbientStrength(POINT_LIGHT1_AMBIENT_STRENGTH);
    pointlight1->setSpecularShininess(POINT_LIGHT1_SPEC_SHININESS);
    pointlight1->setAttenuationConstants(POINT_LIGHT1_CONSTANTS);
    pointlight1->enableShadows();
    incrementNrOfPointLights();
    m_lightComponents.push_back(pointlight1);
}

void
RoomScene::incrementNrOfDirectionalLights()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightingUbo);
    uint32_t offset = offsetof(LightingUbo, nrOfDirectionalLights);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(uint32_t), &DirectionalLight::nrOfInstances);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
RoomScene::incrementNrOfSpotLights()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightingUbo);
    uint32_t offset = offsetof(LightingUbo, nrOfSpotLights);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(uint32_t), &SpotLight::nrOfInstances);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
RoomScene::incrementNrOfPointLights()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightingUbo);
    uint32_t offset = offsetof(LightingUbo, nrOfPointLights);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(uint32_t), &PointLight::nrOfInstances);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static void
RoomSceneDisplayWrapper()
{
    if(globalScene) {
        globalScene->displayScene();
    }
}