find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(BasedVK
        BVKWindow.cpp
        BVKWindow.h
        BVKApp.cpp
        BVKApp.h
        BVKDeviceManager.cpp
        BVKDeviceManager.h
        BVKDevice.cpp
        BVKDevice.h
        BVKSwapChain.cpp
        BVKSwapChain.h
        BVKRenderSystemManager.cpp
        BVKRenderSystemManager.h
        BVKRenderer.cpp
        BVKRenderer.h
)

target_include_directories(BasedVK PRIVATE ../include)
target_link_libraries(BasedVK PRIVATE glfw Vulkan::Vulkan BasedCore plog::plog)