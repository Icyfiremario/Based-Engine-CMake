cmake_minimum_required (VERSION 3.20)

if (WIN32)
  message("Windows detected.")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(BasedEngine VERSION 1.0)

file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include sub-projects.
add_subdirectory(BasedCore)
add_subdirectory(BasedGL)
add_subdirectory(BasedVK)
if (WIN32)
  #add_subdirectory(BasedDX)
endif()
add_subdirectory(shaders)


# find libraries
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)
find_package(GLEW REQUIRED)
find_package(Doxygen)

# Add source to this project's executable.
add_executable(BasedEngine ${SRC} ${HDR})
target_link_libraries(BasedEngine PRIVATE BasedCore BasedGL BasedVK OpenGL::GL glfw Vulkan::Vulkan glm::glm ${GLEW_LIBRARIES})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_options(BasedEngine PRIVATE -Wall -Wextra)
  target_compile_definitions(BasedEngine PRIVATE DEBUG=1)
endif()

if(DOXYGEN_FOUND)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
  doxygen_add_docs(doc ${CMAKE_SOURCE_DIR} COMMENT "Generate documentation")
endif()