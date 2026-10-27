cmake_minimum_required(VERSION 3.22)
project(SunsetScene LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

include(FetchContent)

message(STATUS "Downloading fmt")
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 10.1.1)
FetchContent_MakeAvailable(fmt)

message(STATUS "Downloading spdlog")
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.12.0)
FetchContent_MakeAvailable(spdlog)

file(GLOB_RECURSE SOURCES
    "main.cpp"
    "scene_elements/*.cpp"
    "camera/*.cpp"
    "scenes/*.cpp"
    "utils/*.cpp"
    "assimp_bridge/*.cpp"
    "shaders/*.cpp"
    "cube_map/room_scene/*.cpp"
    "lights/*.cpp"
    "libs/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLUT::GLUT
    OpenGL::GLU
    GLEW::GLEW
    assimp::assimp
    fmt::fmt
    spdlog::spdlog
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/scene_elements
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/scenes
    ${CMAKE_CURRENT_SOURCE_DIR}/scenes/robot_arm
    ${CMAKE_CURRENT_SOURCE_DIR}/scenes/solar_system
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/assimp_bridge
    ${CMAKE_CURRENT_SOURCE_DIR}/camera
    ${CMAKE_CURRENT_SOURCE_DIR}/cube_map/room_scene
    ${CMAKE_CURRENT_SOURCE_DIR}/lights
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb 
)