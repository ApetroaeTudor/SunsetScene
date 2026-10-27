#pragma once

#include <glm/glm.hpp>
#include <array>

// SHADER PATHS CONSTANTS
constexpr const char* const FRAGMENT_SHADER_PATH       = "./shaders/shader_src/fragment.frag";
constexpr const char* const VERTEX_SHADER_PATH         = "./shaders/shader_src/vertex.vert";
constexpr const char* const DEPTH_FRAGMENT_SHADER_PATH =  "./shaders/depth_shaders/fragment.frag";
constexpr const char* const DEPTH_VERTEX_SHADER_PATH   = "./shaders/depth_shaders/vertex.vert";

// MODEL PATHS CONSTANTS

// TABLE CONSTANTS
constexpr const char* const TABLE_MODEL_PATH = "./models/table/Wood_Table.obj"; 
constexpr const float       TABLE_SCALE      = 2.3F;
constexpr const glm::vec3   TABLE_POS        = glm::vec3(5.0F, 0.0F, -10.0F);

// FLOOR CONSTANTS
constexpr const char* const FLOOR_MODEL_PATH = "./models/floor/Floor.fbx"; 
constexpr const float       FLOOR_SCALE      = 0.01f;

// CARPET CONSTANTS
constexpr const char* const CARPET_MODEL_PATH = "./models/carpet/Carpet.fbx";
constexpr const float       CARPET_SCALE      = 1.5f;

// POINT LIGHT LAMP CONSTANTS
constexpr const char* const LAMP_POINTLIGHT_MODEL_PATH         = "./models/lamp_pointlight/Lamp.obj";
constexpr const float       LAMP_POINTLIGHT_SCALE              = 0.7F;
constexpr const glm::vec3   LAMP_POINTLIGHT_ROTATION_DIRECTION = glm::vec3(1, 0, 1);
constexpr const float       LAMP_POINTLIGHT_ROTATION_ANGLE     = -20.0F;
constexpr const glm::vec3   LAMP_POINTLIGHT_POS                = glm::vec3(0.0, 0.93, 0.0); // relative to table

// LAMP SPOTLIGHT CONSTANTS
constexpr const char* const LAMP_SPOTLIGHT_MODEL_PATH = "./models/lamp_spotlight/LampSpotlight.obj"; 
constexpr const float       SPOTLIGHT_LAMP_SCALE      = 2.5F;
constexpr const float       SPOTLIGHT_LAMP_X          = -5.0F;
constexpr const float       SPOTLIGHT_LAMP_Y          = 0.0F;
constexpr const float       SPOTLIGHT_LAMP_Z          = -5.0F;

constexpr const char* const CEILING_MODEL_PATH        ="./models/ceiling/Ceiling.obj";
constexpr const char* const BASIC_WALL_MODEL_PATH     ="./models/basic_wall/BasicWall.obj";
constexpr const char* const WALL_WINDOW_MODEL_PATH    ="./models/wall_window/WallWindow.obj";

// WINDOW CONSTANTS
constexpr const int         INITIAL_WINDOW_POSITION_X = 200;
constexpr const int         INITIAL_WINDOW_POSITION_Y = 200;
constexpr const int         INITIAL_WINDOW_SIZE_W     = 700;
constexpr const int         INITIAL_WINDOW_SIZE_H     = 700;
constexpr const int         SHADOW_WINDOW_SIZE_W      = 5 * 1024;
constexpr const int         SHADOW_WINDOW_SIZE_H      = 5 * 1024;
constexpr const char* const WINDOW_NAME               = "ProiectSPG";

// CAMERA CONSTANTS
constexpr const float CAMERA_SENSITIVITY = 0.15F;
constexpr const float MOVING_SPEED       = 0.1F;
constexpr const float CAMERA_HEIGHT      = 1.1F;

// LIGHTS CONSTANTS --------------------------------------------------------------------------------
constexpr const int ROOM_SCENE_MAX_DIRECTIONAL_LIGHTS = 1;
constexpr const int ROOM_SCENE_MAX_SPOTLIGHTS         = 3;
constexpr const int ROOM_SCENE_MAX_POINTLIGHTS        = 3;

// DIRECTIONAL LIGHT CONSTANTS
constexpr const glm::vec3 DIRECTIONAL_LIGHT_POS              = glm::vec3(3.0, 5.5, 10.0);
constexpr const float     DIRECTIONAL_LIGHT_X                = 0.0F;
constexpr const float     DIRECTIONAL_LIGHT_Y                = 1.0F;
constexpr const float     DIRECTIONAL_LIGHT_Z                = 1.0F;
constexpr const float     DIRECTIONAL_LIGHT_W                = 0.0F;
constexpr const float     DIRECTIONAL_LIGHT_SPEC_SHININESS   = 32.0F;
constexpr const float     DIRECTIONAL_LIGHT_AMBIENT_STRENGTH = 0.7F;

constexpr const float     DIRECTIONAL_LIGHT_SHADOW_NEAR_PLANE = 0.1F;
constexpr const float     DIRECTIONAL_LIGHT_SHADOW_FAR_PLANE  = 100.0F;
constexpr const float     DIRECTIONAL_LIGHT_SHADOW_FRUSTUM_SZ = 60.0F;

// LAMP1 SPOT LIGHT CONSTANTS
constexpr const float LAMP1_SPOT_LIGHT_POS_X = SPOTLIGHT_LAMP_X;
constexpr const float LAMP1_SPOT_LIGHT_POS_Y = SPOTLIGHT_LAMP_Y + 2.0F;
constexpr const float LAMP1_SPOT_LIGHT_POS_Z = SPOTLIGHT_LAMP_Z;

constexpr const float LAMP1_SPOT_LIGHT_DIR_X = 0.001F;
constexpr const float LAMP1_SPOT_LIGHT_DIR_Y = -1.0F;
constexpr const float LAMP1_SPOT_LIGHT_DIR_Z = 0.0F;

constexpr const float LAMP1_SPOT_LIGHT_CUTOFF_ANGLE       = 45.0F;
constexpr const float LAMP1_AMBIENT_LIGHT_STRENGTH        = 0.2F;
constexpr const float LAMP1_SPOT_LIGHT_OUTER_CUTOFF_ANGLE = LAMP1_SPOT_LIGHT_CUTOFF_ANGLE + 15.0F;
constexpr const float LAMP1_SPOT_LIGHT_SPEC_SNININESS     = 8.0F;

constexpr const float SPOT_LIGHT_SHADOW_NEAR_PLANE = 0.5F;
constexpr const float SPOT_LIGHT_SHADOW_FAR_PLANE  = 50.0F;
constexpr const float SPOT_LIGHT_SHADOW_CONE_ANGLE        = LAMP1_SPOT_LIGHT_OUTER_CUTOFF_ANGLE; 

// TABLE 2 -- RELATIVE TO LAMP1 SPOT LIGHT
constexpr const glm::vec3 TABLE2_POS      = glm::vec3(1.0F, 0.0F, 1.0F);
constexpr       float TABLE2_ROT_ANGLE    = 30.0F;
constexpr       glm::vec3 TABLE2_ROT_AXES = glm::vec3(0.0F, 1.0F, 0.0F);  

// LAMP POINT LIGHT CONSTATNS
constexpr const glm::vec3           POINT_LIGHT1_POS              = glm::vec3(TABLE_POS + LAMP_POINTLIGHT_POS + glm::vec3(0,0.7F,0));
constexpr const std::array<float,3> POINT_LIGHT1_CONSTANTS        = {1.0F, 0.14F, 0.07F}; // KC - KL - KQ
constexpr const float               POINT_LIGHT1_SPEC_SHININESS   = 8.0F;
constexpr const float               POINT_LIGHT1_AMBIENT_STRENGTH = 0.5F; 

// SHADOW POSITION CONSTANTS
// i will use 25 onwards for shadow maps
constexpr const int DIRECTIONAL_LIGHT_MAP_1 = 25;

constexpr const int SPOT_LIGHT_MAP_1 = 26;
constexpr const int SPOT_LIGHT_MAP_2 = 27;
constexpr const int SPOT_LIGHT_MAP_3 = 28;

constexpr const int POINT_LIGHT_MAP_1 = 29;
constexpr const int POINT_LIGHT_MAP_2 = 30;
constexpr const int POINT_LIGHT_MAP_3 = 31;
// ~ LIGHTS CONSTANTS --------------------------------------------------------------------------------


