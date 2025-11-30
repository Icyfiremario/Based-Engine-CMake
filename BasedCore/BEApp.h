#ifndef BEAPP_H
#define BEAPP_H

// STD
#include <memory>
#include <vector>

// BasedCore
#include "Common/BEDevice.h"
#include "Common/BEWindow.h"
#include "Managers/BEDeviceManager.h"

class BEApp
{

public:

    BEApp() = default;
    ~BEApp() = default;

private:

    BEDeviceManager* deviceManager = BEDeviceManager::getInstance();

    std::unique_ptr<BasedEngine::Common::BEWindow> appWindow;
    std::unique_ptr<BasedEngine::Common::BEDevice> appDevice;

};


#endif //BEAPP_H