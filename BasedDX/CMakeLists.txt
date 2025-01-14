file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

find_package(DirectX REQUIRED)

add_library(BasedDX ${SRC} ${HDR})

target_link_libraries(BasedDX DirectX::DirectX)