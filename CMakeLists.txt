include(FetchContent)

cmake_minimum_required (VERSION 3.20)

if (WIN32)
  set(DIRECTX_AVAILABLE True)
endif()

project(BasedEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")
add_compile_definitions($<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:RelWithDebInfo>:DEBUG>)
if (DIRECTX_AVAILABLE)
    add_compile_definitions(DIRECTX_AVAILABLE WINDOWS)
endif (DIRECTX_AVAILABLE)

# Include sub-projects.
add_subdirectory(BasedCore)
add_subdirectory(BasedGL)
add_subdirectory(third_party/glad)
add_subdirectory(BasedVK)
if (DIRECTX_AVAILABLE)
  add_subdirectory(BasedDX)
endif()
add_subdirectory(shaders)
add_subdirectory(models)


# Find libraries
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)
#find_package(GLEW REQUIRED)
if(DIRECTX_AVAILABLE)
  find_package(DirectX-Headers REQUIRED)
endif (DIRECTX_AVAILABLE)

find_package(libzip REQUIRED)

# Fetch external libraries
FetchContent_Declare(
        plog
        GIT_REPOSITORY https://github.com/SergiusTheBest/plog
        GIT_TAG        1.1.11
        GIT_SHALLOW    true
)
FetchContent_MakeAvailable(plog)

# Extras
find_package(Doxygen)

# Add source to this project's executable.
add_executable(BasedEngine main.cpp)

target_compile_options(BasedEngine PRIVATE -Wall $<$<CONFIG:Debug>: -g -O0> $<$<CONFIG:Release>: -O3> $<$<CONFIG:RelWithDebInfo>: -O2 -g> $<$<CONFIG:MinSizeRel>: -Os>)

target_link_libraries(BasedEngine PRIVATE BasedCore BasedGL BasedVK OpenGL::GL glfw Vulkan::Vulkan glm::glm glad plog::plog libzip::zip)

if (DIRECTX_AVAILABLE)
  target_link_libraries(BasedEngine PRIVATE BasedDX user32 gdi32 kernel32 d3d12 dxgi dxguid d3dcompiler)
endif (DIRECTX_AVAILABLE)

if (MSVC)
    add_compile_options(BasedEngine PRIVATE /MP /W4 $<$<CONFIG:Debug>: /Od /ZI>)
    add_link_options(BasedEngine PRIVATE $<$<CONFIG:Debug>: /INCREMENTAL>)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(BasedEngine PRIVATE -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(BasedEngine PRIVATE -Wall -Wextra -pedantic -Wextra_tokens -fcolor_diagnostics $<$<CONFIG:Debug>: -fsanitize=undefined -fsanitize=address>)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_compile_options(BasedEngine PRIVATE -Wall -Wextra -pedantic -Wextra_tokens -fcolor-diagnostics $<$<CONFIG:Debug>: -fsanitize=undefined -fsanitize=address>)
else()
    message(FATAL "Unsupported compiler!")
endif()

add_dependencies(BasedEngine CompileShaders CopyModels)

if(DOXYGEN_FOUND)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
  doxygen_add_docs(doc ${CMAKE_SOURCE_DIR} COMMENT "Generate documentation")
endif()
