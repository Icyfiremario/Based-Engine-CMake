# Based Engine

## About
Based Engine is an open source game engine written in C++. It uses glfw for window management to allow support for several rendering APIs. The current plan is to create an open source C++ based game building application similar to unity.

## Status

Very unstable. Weird bugs with rendering freezing and the camera teleporting for unknown reasons.

## Support
### Windows
- Vulkan
- OpenGL
- DirectX [^1]

### Linux
- Vulkan
- OpenGL

### Apple
- OpenGL

## To Do
### Core
- [x] Window resizing
- [ ] Rendering API binding
- [x] Add Plog for logging
- [ ] Create game object component system

### Vulkan
- [x] SwapChain
- [ ] Pipeline
- [ ] Shaders [^2]
- [ ] Models
- [x] Device info
- [ ] Descriptors
- [ ] Render systems

### OpenGL
- [ ] General implementation

### DirectX
- [ ] HWND binding

### Editor
- [ ] 3D editor
- [ ] Game object viewer
- [ ] Included game object components. (Models, textures, scripts, etc.)

## Required packages
- GLM
- GLEW (OpenGL)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- CMake
- GLSLC
- GLFW3
- [Tiny object loader](https://github.com/tinyobjloader/tinyobjloader) [^3]

## Supported compilers
### Windows
- MSYS2 GCC
- MSYS2 Clang
- MSVC 17

### Linux
- GCC
- Clang

[^1]: This will be done by grabbing the HWND from GLFW. IDK the drawbacks to doing this, but it makes my life simpler.

[^2]: Custom shaders will be done using render systems. There will be a parent render system class that you override with your own functions and shaders. An example of this is between the Pointlight render system and the BVKRenderSystem. The Pointlight system is almost exactly the same as BVKRenderSystem but with some changes to the render function and the addition of an update function. In the future, the BVKRenderSystem class will be used as a parent class for all render systems and the current implementation will be the BVKBasicRenderSystem.

[^3]: This package supports loading wavefront object models. I will replace this in the future with an in-house model loader in order to support more model formats. If using you are using MSVC, models may need to be renamed to .wobj in order to prevent MSVC from trying to link them into the program.
