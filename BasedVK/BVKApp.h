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
    BVKApp(int width, int height, const char* title);
    ~BVKApp() override;

    void run() override;

private:

    std::unique_ptr<BVKWindow> appWindow = nullptr;
};


#endif // BVKAPP_H