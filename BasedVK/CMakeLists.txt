file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(BasedVK ${SRC} ${HDR})
target_link_libraries(BasedVK PRIVATE glfw Vulkan::Vulkan)