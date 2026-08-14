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
        BVKRenderSystem.cpp
        BVKRenderSystem.h
        BVKPipeline.cpp
        BVKPipeline.h
        BVKModel.cpp
        BVKModel.h
        BVKBuffer.cpp
        BVKBuffer.h
        BVKFrameInfo.h
        BVKObject.cpp
        BVKObject.h
)

target_include_directories(BasedVK PRIVATE ../include)
target_link_libraries(BasedVK PRIVATE glfw Vulkan::Vulkan BasedCore plog::plog)