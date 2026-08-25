find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED) # Included because the BEWindow class includes the GLFW Vulkan headers for the BVKWindow class

add_library(BasedGL
        BGLWindow.cpp
        BGLWindow.h
        BGLApp.cpp
        BGLApp.h
        BGLShader.cpp
        BGLShader.h
)

target_include_directories(BasedGL PRIVATE ${OPENGL_INCLUDE_DIR} ../include)
target_link_libraries(BasedGL PRIVATE glad OpenGL::GL Vulkan::Vulkan glfw BasedCore plog::plog)