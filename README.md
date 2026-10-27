# SunsetScene

SunsetScene is a C++23/OpenGL project for rendering and interacting with 3D scenes. It was developed as a computer graphics project, with a focus on scene organization, 3D transformations, lighting, shaders and model loading.

The code is split into reusable C++ components for managing cameras, scene objects, lighting and OpenGL resources.

## Technical Overview

### OpenGL Rendering

The project uses OpenGL for hardware-accelerated 3D rendering, with GLEW for accessing OpenGL functions and GLUT for window management, input handling and the application event loop.

The rendering code covers several core graphics concepts:

- GLSL vertex and fragment shaders
- Model, view and projection transformations
- Vertex geometry and texture handling
- Camera-based scene navigation
- Lighting and material properties
- Cubemap textures and environment rendering
- Loading external 3D models using Assimp

GLM is used for vector and matrix operations, including object transformations and camera calculations.

### C++ Architecture

The project uses C++23 and separates the application into components responsible for different parts of the rendering process.

Scene-specific code is kept separate from common functionality such as cameras, shaders, lighting and model loading. This allows different scenes to reuse the same rendering utilities and object implementations.

The application uses C++ classes to represent scene components and standard library facilities such as `std::shared_ptr` for object ownership.

The main entry point initializes the application and logging system, creates a `RoomScene` instance, activates it and starts the GLUT event loop.

### Scene Management

The project contains implementations for different types of scenes, including a room environment, a robotic arm and a solar system.

Scene implementations are organized under `scenes/`, while common scene objects and rendering components are stored separately.

The robotic arm and solar system scenes provide a way to work with object transformations and relationships between multiple 3D objects.

### Shaders and Lighting

GLSL shaders are used to control vertex processing and fragment rendering.

The shader and lighting code is organized into separate modules, keeping OpenGL shader operations independent from scene definitions.

The project also includes support for working with materials, textures and different lighting configurations.

### Model Loading

Assimp is used to import external 3D assets. The `assimp_bridge/` module separates model-import functionality from the rest of the rendering code.

Imported model data can then be used by the OpenGL rendering components alongside geometry defined directly in the application.

## Dependencies

| Library | Purpose |
|---|---|
| C++23 | Application code |
| OpenGL | 3D rendering |
| GLEW | OpenGL function loading |
| FreeGLUT / GLUT | Window management and input |
| GLM | Vector and matrix mathematics |
| Assimp | 3D model importing |
| stb | Image and texture utilities |
| spdlog | Application logging |
| fmt | String formatting |
| CMake | Build configuration |

The project uses CMake FetchContent to download `fmt` (10.1.1) and `spdlog` (1.12.0). The other libraries must be available on the system.

## Building

Requires CMake 3.26 or newer, a C++23-compatible compiler and the required graphics libraries.

```bash
git clone https://github.com/ApetroaeTudor/SunsetScene.git
cd SunsetScene

cmake -S . -B build
cmake --build build
```

The CMake configuration currently builds a Debug executable named `SunsetScene`.

On Linux, the executable can be started from the repository root with:

```bash
./build/SunsetScene
```

Running from the project directory helps preserve relative paths used for assets and resources.

## Project Structure

```text
SunsetScene/
├── assimp_bridge/       # Assimp model loading
├── camera/              # Camera implementation
├── cube_map/
│   └── room_scene/      # Room scene and cubemap code
├── lights/              # Lighting components
├── libs/                # Supporting libraries
├── models/              # 3D model assets
├── resources/           # Textures and other assets
├── scene_elements/      # Reusable scene objects
├── scenes/              # Scene implementations
│   ├── robot_arm/
│   └── solar_system/
├── shaders/             # GLSL shader management
├── utils/               # Common utilities
├── main.cpp             # Application entry point
└── CMakeLists.txt        # Build configuration
```

## Notes

This is a standalone graphics project intended for experimenting with OpenGL and C++ rather than a general-purpose rendering engine.

The current application entry point launches the room scene. Other scene implementations are included in the source tree.
