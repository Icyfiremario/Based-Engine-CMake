find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(BasedCore
        Common/BEWindow.cpp
        Common/BEWindow.h
        Common/BEModel.cpp
        Common/BEModel.h
        BEUtils.h
        Common/BEObject.cpp
        Common/BEObject.h
        Common/BEDevice.cpp
        Common/BEDevice.h
)

target_link_libraries(BasedCore PRIVATE glfw Vulkan::Vulkan OpenGL::GL GLEW::GLEW d3d12 dxgi d3dcompiler)