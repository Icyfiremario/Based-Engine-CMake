find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_library(BasedDX
        BDXWindow.cpp
        BDXWindow.h
        stdafx.h
        stdafx.cpp
        BDXDevice.cpp
        BDXDevice.h
)

target_link_libraries(BasedDX PRIVATE user32 gdi32 kernel32 glfw Vulkan::Vulkan d3d12 dxgi dxguid d3dcompiler)