file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_library(BasedGL ${SRC} ${HDR})

target_include_directories(BasedGL PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(BasedGL OpenGL::GL ${GLEW_LIBRARIES} glfw)