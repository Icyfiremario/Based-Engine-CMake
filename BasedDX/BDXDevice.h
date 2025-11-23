#ifndef BDXDEVICE_H
#define BDXDEVICE_H

// BasedCore
#include "../BasedCore/BEWindow.h"

class BDXDevice
{
public:

private:
    Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter;
};


#endif // BDXDEVICE_H