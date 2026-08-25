find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)

add_library(BasedCore
        Common/BEWindow.cpp
        Common/BEWindow.h
        Common/BEModel.cpp
        Common/BEModel.h
        BEUtils.h
        Common/BEObject.cpp
        Common/BEObject.h
        Managers/BEAppManager.cpp
        Managers/BEAppManager.h
        Common/BEApp.cpp
        Common/BEApp.h
)

target_include_directories(BasedCore PRIVATE ../include)
target_link_libraries(BasedCore PRIVATE glfw Vulkan::Vulkan glad OpenGL::GL plog::plog)

if (DIRECTX_AVAILABLE)
    target_link_libraries(BasedCore PRIVATE d3d12 dxgi d3dcompiler)
endif (DIRECTX_AVAILABLE)