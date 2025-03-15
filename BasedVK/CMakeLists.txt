file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(BasedVK ${SRC} ${HDR})
target_link_libraries(BasedVK PRIVATE glfw Vulkan::Vulkan)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_definitions(BasedVK PRIVATE DEBUG=1)
endif()
