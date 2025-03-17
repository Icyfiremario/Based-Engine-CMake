file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

add_library(BasedDX ${SRC} ${HDR})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_definitions(BasedDX PRIVATE DEBUG=1)
endif()
