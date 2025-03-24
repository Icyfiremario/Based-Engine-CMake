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

### Vulkan
- [x] Swapchain
- [ ] Pipeline
- [ ] Shaders
- [ ] Models
- [x] Device info
- [ ] Descriptors

### OpenGL
- [ ] General implementation

## Required packages
- GLM
- GLEW
- Vulkan
- OpenGL
- CMake
- GLSLC
- GLFW3

[^1]: This requires using windows.h and doing window management that way. I am unfamillar and it would require weird cmake stuff. I do plan to add this but it will be way down the line.