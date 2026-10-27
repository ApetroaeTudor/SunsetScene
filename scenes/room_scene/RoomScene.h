#pragma once

#include "Camera.h"
#include "CubeMap.h"
#include "ILightComponent.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "lights/DirectionalLight.h"
#include "scene_elements/ISceneComponent.h"
#include "IScene.h"
#include <memory>
#include <vector>
#include "constants.h"
#include "shaders/ShaderProgram.h"

class RoomScene: public IScene, public std::enable_shared_from_this<RoomScene>
{
private:
    static constexpr const char* const LOGGING_NAME = "scenes::RoomScene";

private:
    // must be 16 byte alligned
    struct LightingUbo
    {
        DirectionalLight::DirectionalLightGPU directionalLights[ROOM_SCENE_MAX_DIRECTIONAL_LIGHTS];
        
        // 16
        uint32_t nrOfDirectionalLights = 0;
        uint32_t padding[3];
    
        SpotLight::SpotLightGPU spotLights[ROOM_SCENE_MAX_SPOTLIGHTS];

        //16
        uint32_t nrOfSpotLights = 0;
        uint32_t padding2[3];

        PointLight::PointLightGPU pointLights[ROOM_SCENE_MAX_POINTLIGHTS];

        //16
        uint32_t nrOfPointLights = 0;
        uint32_t padding3[3];

    };
    static_assert(sizeof(LightingUbo) % 16 == 0, "UBO structure in Room Scene is not 16 byte alligned");

public:
    RoomScene();

    void displayScene()override;

    void initializeScene()override;
    
    void startUpdate(int value)override;
    
    void activateScene()override;

    void initializeLightingUbo()override;

    void initializeLights()override;

    ~RoomScene()override = default;

private:
    void incrementNrOfDirectionalLights();
    void incrementNrOfSpotLights();
    void incrementNrOfPointLights();
    void initializeDepthTestingShader();

private:
    ISceneComponentPtr m_topLevelComponent;
    ShaderProgram      m_depthTestingShader;
    Camera             m_camera;
    CubeMap            m_cubeMap;
    GLuint             m_lightingUbo;
    std::vector<ILightComponentPtr> m_lightComponents;
};

void roomSceneKeyboardCallback(unsigned char key, int x, int y);