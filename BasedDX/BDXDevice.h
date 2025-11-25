#ifndef BDXDEVICE_H
#define BDXDEVICE_H

// BasedCore
#include "../BasedCore/Common/BEDevice.h"

// BasedDX
#include "BDXWindow.h"

class BDXDevice : BasedEngine::Common::BEDevice
{
public:

private:
    Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter;
};


#endif // BDXDEVICE_H