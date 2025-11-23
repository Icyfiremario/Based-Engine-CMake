add_library(BasedDX
        BDXModel.h
        BDXModel.cpp
        BDXDevice.cpp
        BDXDevice.h
)

target_link_libraries(BasedDX PRIVATE d3d12 dxgi d3dcompiler)