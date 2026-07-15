#ifndef BVKAPP_H
#define BVKAPP_H

// BasedCore
#include "../BasedCore/Common/BEApp.h"

// BasedVK
#include "BVKDeviceManager.h"

class BVKApp : public BEApp
{
public:

    BVKApp();
    ~BVKApp() override;

    void run() override;

private:

    std::unique_ptr<BVKWindow> appWindow = nullptr;
    std::unique_ptr<BVKDevice> mainWindow = nullptr;
};


#endif // BVKAPP_H