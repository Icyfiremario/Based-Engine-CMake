file(GLOB SRC CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB HDR CONFIGURE_DEPENDS "*.h" "*.hpp")

#set(SRC BEapp.cpp BECamera.cpp BEKeyboardController.cpp BEwindow.cpp)
#set(HDR BasedConfig.h BEapp.h BECamera.h BEenums.h BEKeyboardController.h BEwindow.h)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(BasedCore ${SRC} ${HDR})
target_link_libraries(BasedCore PRIVATE glfw Vulkan::Vulkan OpenGL::GL GLEW::GLEW)