file(GLOB SRC CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB HDR CONFIGURE_DEPENDS "*.h" "*.hpp")

#set(SRC BVKBuffer.cpp BVKDescriptors.cpp BVKDevice.cpp BVKModel.cpp BVKObject.cpp BVKPipeline.cpp BVKPointLightRenderSystem.cpp BVKRenderer.cpp BVKRenderSystem.cpp BVKSwapchain.cpp)
#set(HDR BVKBuffer.h BVKDescriptors.h BVKDevice.h BVKFrameInfo.h BVKModel.h BVKObject.h BVKPipeline.h BVKPointLightRenderSystem.h BVKRenderer.h BVKRenderSystem.h BVKSwapchain.h BVKUtils.h)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(BasedVK ${SRC} ${HDR})
target_link_libraries(BasedVK PRIVATE glfw Vulkan::Vulkan)