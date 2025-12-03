#ifndef BDXDEVICE_H
#define BDXDEVICE_H

// BasedCore
#include "../BasedCore/Common/BEDevice.h"

// BasedDX
#include "stdafx.h"

class BDXDevice : BasedEngine::Common::BEDevice
{
public:


private:

    Microsoft::WRL::ComPtr<ID3D12Device> device;
};


#endif // BDXDEVICE_H