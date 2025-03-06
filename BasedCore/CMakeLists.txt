file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(glfw3 REQUIRED)

add_library(BasedCore ${SRC} ${HDR})
target_link_libraries(BasedCore PRIVATE glfw)
