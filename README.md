# Based Engine

## About
Based Engine is an open source game engine written in C++. It uses glfw for window management to allow support for several rendering APIs.

## Support
### Windows
- Vulkan
- OpenGL
- DirectX [^1]

### Linux
- Vulkan
- OpenGL

### Apple
Apple support is not planned and likely not going to happen.

## To Do
### Core
- [x] Window resizing
- [x] Rendering API binding
- [ ] Add Plog for logging

### Vulkan
- [x] Swapchain
- [x] Pipeline
- [x] Shaders [^2]
- [x] Models
- [x] Device info
- [ ] Descriptors

### OpenGL
- [ ] General implementation

### DirectX
- [ ] WinMain entry

## Required packages
- GLM
- GLEW
- Vulkan
- OpenGL
- CMake
- GLSLC
- GLFW3
- Tiny object loader [^3]

## Supported compilers
### Windows
- MSYS2 GCC
- MSYS2 Clang

### Linux
- GCC
- Clang

[^1]: This requires using windows.h and doing window management that way. I am unfamillar and it would require weird cmake stuff. I do plan to add this but it will be way down the line. I may end up just using DirectX translation layers if I cannot figure out how to bind OpenGL and Vulkan to windows.h windows.

[^2]: Shaders are loaded into the pipeline and used but there is currently no way to add custom shaders and add them to the rendering pipeline.

[^3]: This package supports loading wavefront object models. I will replace this in the future with a in-house model loader in order to support more model formats. If using you are using MSVC models may need to be renamed to .wobj in order to prevent MSVC from trying to link them into the program. IDK for sure though because I haven't got compiling with MSVC to work yet.
