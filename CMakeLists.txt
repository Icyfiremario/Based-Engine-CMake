cmake_minimum_required (VERSION 3.11)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(BasedEngine VERSION 1.0)

file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

# Include sub-projects.
add_subdirectory(BasedGL)
add_subdirectory(BasedVK)

# Add source to this project's executable.
add_executable(BasedEngine ${SRC} ${HDR})
target_link_libraries(BasedEngine PRIVATE BasedGL BasedVK)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BasedEngine PROPERTY CXX_STANDARD 20)
endif()