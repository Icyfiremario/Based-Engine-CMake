find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(BasedVK
        BVKBuffer.cpp
        BVKBuffer.h
        BVKWindow.cpp
        BVKWindow.h
        BVKApp.cpp
        BVKApp.h
        BVKDeviceManager.cpp
        BVKDeviceManager.h
        BVKDevice.cpp
        BVKDevice.h
        BVKSwapchain.cpp
        BVKSwapchain.h
        BVKRenderSystemManager.cpp
        BVKRenderSystemManager.h
)

target_include_directories(BasedVK PRIVATE ../include)
target_link_libraries(BasedVK PRIVATE glfw Vulkan::Vulkan BasedCore)