#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(800, 600, "Based Vulkan");
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);

    resetDeviceRenderer();

    globalPool = BVKDescriptorPool::Builder(*deviceManager->getDevicePtr()).setMaxSets(BVKSwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, BVKSwapChain::MAX_FRAMES_IN_FLIGHT).build();
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);

    resetDeviceRenderer();

    globalPool = BVKDescriptorPool::Builder(*deviceManager->getDevicePtr()).setMaxSets(BVKSwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, BVKSwapChain::MAX_FRAMES_IN_FLIGHT).build();
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

	static bool keyFPressed = false;

    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            PLOGI << "Escape key pressed. Closing app...";
            glfwSetWindowShouldClose(appWindow->getWindow(), true);
        }

        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_F) == GLFW_PRESS && !keyFPressed)
        {
            PLOGI << "Attempting to switch GPUs.";

            deviceIndex++;

            if (deviceIndex >= maxDeviceIndex)
            {
                deviceIndex = 0;
            }

            if (!deviceManager->switchDevice(deviceIndex))
            {
                PLOGF << "Failed to switch app device.";
                throw std::runtime_error("Failed to switch app device!");
            }
            
            resetDeviceRenderer();

            PLOGI << "Switched device to device: " << deviceIndex;
			keyFPressed = true;
        }

        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_F) == GLFW_RELEASE && keyFPressed)
        {
            keyFPressed = false;
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
