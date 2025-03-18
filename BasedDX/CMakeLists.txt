file(GLOB SRC "*.c" "*.cpp")
file(GLOB HDR "*.h" "*.hpp")

add_library(BasedDX ${SRC} ${HDR})