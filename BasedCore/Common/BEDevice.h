#ifndef BEDEVICE_H
#define BEDEVICE_H

// STD

// BasedCore
#include "BEWindow.h"

namespace BasedEngine::Common
{
    /// @brief Base class so I don't have to do as many shenanigans
    class BEDevice
    {
    public:

        ~BEDevice() = default;

        BEDevice(const BEDevice&) = delete;
        BEDevice& operator=(const BEDevice&) = delete;
        BEDevice(BEDevice&&) = delete;
        BEDevice& operator=(BEDevice&&) = delete;

    };
}

#endif // BEDEVICE_H
