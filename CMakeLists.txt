cmake_minimum_required (VERSION 4.0)

if (WIN32)
  message("Windows detected.")
  set(DIRECTX_AVAILABLE True)
endif()

project(BasedEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(STATUS "Using GNU style compiler flags")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using Clang extensions")
    set(CMAKE_CXX_FLAGS_ALL "-Wall -Wextra -pedantic -Wextra-tokens -fcolor-diagnostics")

    if (DIRECTX_AVAILABLE)
      set(CMAKE_CXX_FLAGS_ALL "${CMAKE_CXX_FLAGS_ALL} -DDIRECTX_AVAILABLE")
    endif (DIRECTX_AVAILABLE)

    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64|arm64|AARCH64|ARM64")
      set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS_ALL} -DDEBUG")
    else ()
      message(STATUS "Using sanitizers")
      set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=undefined -fsanitize=address ${CMAKE_CXX_FLAGS_ALL} -DDEBUG")
    endif ()

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_ALL} -O2")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os ${CMAKE_CXX_FLAGS_ALL}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${CMAKE_CXX_FLAGS_ALL} -DDEBUG")
  else()
    set(CMAKE_CXX_FLAGS_ALL "-Wall -Wextra -Wpedantic")

    set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS_ALL} -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_ALL} -O2")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os ${CMAKE_CXX_FLAGS_ALL}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${CMAKE_CXX_FLAGS_ALL} -DDEBUG")
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message(STATUS "Using MSVC style compiler flags")
  set(CMAKE_CXX_FLAGS_ALL "/Wall /MP")

  set(CMAKE_CXX_FLAGS_DEBUG "/W4 /Zi /Od /DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/W4 /O2")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "/W4 /Os")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/W4 /Zi /O2 /DDEBUG")
  message(FATAL_ERROR "MSVC compiler is not supported yet.\nI have not written into the build chain the proper methods for finding the required libraries and linking them.")
else()
  message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Include sub-projects.
add_subdirectory(BasedCore)
add_subdirectory(BasedGL)
add_subdirectory(BasedVK)
if (DIRECTX_AVAILABLE)
  add_subdirectory(BasedDX)
endif()
add_subdirectory(shaders)
add_subdirectory(models)


# find libraries
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)
find_package(GLEW REQUIRED)
if(DIRECTX_AVAILABLE)
  find_package(DirectX-Headers REQUIRED)
endif (DIRECTX_AVAILABLE)

# Extras
find_package(Doxygen)

# Add source to this project's executable.
add_executable(BasedEngine main.cpp)
target_include_directories(BasedEngine PRIVATE Include)
target_link_libraries(BasedEngine PRIVATE BasedCore BasedGL BasedVK OpenGL::GL glfw Vulkan::Vulkan glm::glm ${GLEW_LIBRARIES} )
if (DIRECTX_AVAILABLE)
  target_link_libraries(BasedEngine PRIVATE user32 gdi32 kernel32 d3d12 dxgi dxguid d3dcompiler)
endif (DIRECTX_AVAILABLE)
add_dependencies(BasedEngine CompileShaders CopyModels)

#if(WIN32)
#  target_compile_definitions(BasedEngine PRIVATE USE_WINDOWS=1)
#endif(WIN32)

if(DOXYGEN_FOUND)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
  doxygen_add_docs(doc ${CMAKE_SOURCE_DIR} COMMENT "Generate documentation")
endif()