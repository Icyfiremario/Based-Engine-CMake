file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(BasedCore ${SRC} ${HDR})
target_link_libraries(BasedCore PRIVATE glfw Vulkan::Vulkan OpenGL::GL GLEW::GLEW)
