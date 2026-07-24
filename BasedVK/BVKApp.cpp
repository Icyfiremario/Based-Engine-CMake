#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 400, "Based Vulkan");

    const auto deviceManager = BVKDeviceManager::getInstance(appWindow.get());
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);

    const auto deviceManager = BVKDeviceManager::getInstance(appWindow.get());
}

BVKApp::~BVKApp()
{
    appWindow.reset();
}

void BVKApp::run()
{
    static int deviceIndex = 0;
    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            PLOGI << "Escape key pressed. Closing app...";
            glfwSetWindowShouldClose(appWindow->getWindow(), true);
        }

        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        {
            PLOGI << "Page up key pressed. Increasing GPU index.";
            deviceIndex++;
            const auto deviceManager = BVKDeviceManager::getInstance();
            deviceManager->setDeviceIndex(deviceIndex);
        }

        glfwPollEvents();
    }
}
