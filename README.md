# Based Engine

## About
Based Engine is an open source game engine written in C++. It uses glfw for window management to allow support for several rendering APIs.

## Support
### Windows
- Vulkan
- OpenGL
- DirectX [^1]

## To Do
### Core
- [ ] Refactor the core window class to be a parent class
- [ ] Window resizing
- [ ] Rendering API binding

### Vulkan
- [ ] Vulkan specific window class
- [ ] Swapchain
- [ ] Pipeline
- [ ] Shaders
- [ ] Models
- [ ] Device info
- [ ] Descriptors

### OpenGL
- [ ] General implementation

[^1]: This requires using windows.h and doing window management that way. I am unfamillar and it would require weird cmake stuff. I do plan to add this but it will be way down the line.