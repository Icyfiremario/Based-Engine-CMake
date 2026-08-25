#ifndef BVKAPP_H
#define BVKAPP_H

// BasedCore
#include "../BasedCore/Common/BEApp.h"
#include "../BasedCore/Common/BECamera.h"

// BasedVK
#include "BVKDeviceManager.h"
#include "BVKRenderer.h"
#include "BVKDescriptors.h"
#include "BVKBuffer.h"
#include "BVKFrameInfo.h"
#include "BVKRenderSystem.h"

class BVKApp : public BEApp
{
public:

    BVKApp();
    BVKApp(int width, int height, const char* title);
    ~BVKApp() override;

    void run() override;

private:

    std::unique_ptr<BVKWindow> appWindow = nullptr;
    std::unique_ptr<BVKDeviceManager> deviceManager = nullptr;
    std::unique_ptr<BVKRenderer> appRenderer = nullptr;

    std::unique_ptr<BVKDescriptorPool> globalPool;

    BVKObject::Map appObjects;

    void resetDeviceRenderer();

    void createAppResources();
};


#endif // BVKAPP_H