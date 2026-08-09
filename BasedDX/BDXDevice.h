#ifndef BDXDEVICE_H
#define BDXDEVICE_H

// BasedDX
#include "stdafx.h"

class BDXDevice
{
public:
    BDXDevice();

private:

    Microsoft::WRL::ComPtr<ID3D12Device> device;
};


#endif // BDXDEVICE_H