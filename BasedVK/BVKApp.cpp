#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 400, "Based Vulkan");
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);
    appRenderer = std::make_unique<BVKRenderer>(*appWindow, *deviceManager->getDevicePtr());
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);
    appRenderer = std::make_unique<BVKRenderer>(*appWindow, *deviceManager->getDevicePtr());
}

BVKApp::~BVKApp()
{
    appRenderer.reset();
    deviceManager.reset();
    appWindow.reset();
}

void BVKApp::run()
{
    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            PLOGI << "Escape key pressed. Closing app...";
            glfwSetWindowShouldClose(appWindow->getWindow(), true);
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
