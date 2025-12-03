find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(BasedVK
        BVKModel.cpp
        BVKModel.h
        BVKDevice.cpp
        BVKDevice.h
        BVKBuffer.cpp
        BVKBuffer.h
        BVKWindow.cpp
        BVKWindow.h
        BVKApp.cpp
        BVKApp.h
        BVKDeviceManager.cpp
        BVKDeviceManager.h)

target_link_libraries(BasedVK PRIVATE glfw Vulkan::Vulkan)