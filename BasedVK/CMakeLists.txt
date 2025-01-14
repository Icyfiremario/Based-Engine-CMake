file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(Vulkan REQUIRED)

add_library(BasedVK ${SRC} ${HDR})

target_link_libraries(BasedVK Vulkan::Vulkan)
