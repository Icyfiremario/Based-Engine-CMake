file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(OpenGL REQUIRED)

add_library(BasedGL ${SRC} ${HDR})

target_include_directories(BasedGL PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(BasedGL OpenGL::GL)