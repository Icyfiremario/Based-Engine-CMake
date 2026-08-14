#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 400, "Based Vulkan");
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);

    resetDeviceRenderer();
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);

    resetDeviceRenderer();
}

BVKApp::~BVKApp()
{
    appRenderer.reset();
    deviceManager.reset();
    appWindow.reset();
}

void BVKApp::run()
{
    static int deviceIndex = 0;
    const int maxDeviceIndex = static_cast<int>(deviceManager->getDeviceList()->size());

    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            PLOGI << "Escape key pressed. Closing app...";
            glfwSetWindowShouldClose(appWindow->getWindow(), true);
        }

        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_F) == GLFW_PRESS)
        {
            PLOGI << "Attempting to switch GPUs.";

            deviceIndex++;

            if (deviceIndex > maxDeviceIndex)
            {
                deviceIndex = 0;
            }

            deviceManager->switchDevice(deviceIndex);
            resetDeviceRenderer();

            PLOGI << "Switched device to device: " << deviceIndex;
        }

        glfwPollEvents();

        if (const auto commandBuffer = appRenderer->beginFrame())
        {
            appRenderer->beginSwapChainRenderPass(commandBuffer);

            appRenderer->endSwapChainRenderPass(commandBuffer);
            appRenderer->endFrame();
        }

        vkDeviceWaitIdle(deviceManager->getDevicePtr()->getDevice());
    }
}

void BVKApp::resetDeviceRenderer()
{
    appRenderer.reset();
    appRenderer = nullptr;
    appRenderer = std::make_unique<BVKRenderer>(*appWindow, *deviceManager->getDevicePtr());
}
