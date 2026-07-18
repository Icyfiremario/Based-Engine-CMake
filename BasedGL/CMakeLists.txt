find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED) # Included because the BEWindow class includes the GLFW Vulkan headers for the BVKWindow class

add_library(BasedGL
        BGLModel.h
        BGLModel.cpp
        BGLWindow.cpp
        BGLWindow.h
)

target_include_directories(BasedGL PRIVATE ${OPENGL_INCLUDE_DIR} ../include)
target_link_libraries(BasedGL OpenGL::GL Vulkan::Vulkan glfw GLEW::GLEW BasedCore)