#ifndef BEDEVICE_H
#define BEDEVICE_H

// STD

// BasedCore
#include "../BEWindow.h"

namespace BasedEngine::Common
{
    /// @brief Base class so I don't have to do as many shinanigans
    class BEDevice
    {
    public:

        ~BEDevice() = default;
    };
}

#endif // BEDEVICE_H
