#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 400, "Based Vulkan");

    const auto deviceManager = BVKDeviceManager::getInstance();
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);

    const auto deviceManager = BVKDeviceManager::getInstance();
}

BVKApp::~BVKApp()
{
    appWindow.reset();
}

void BVKApp::run()
{
    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(appWindow->getWindow(), true);
        }

        glfwPollEvents();
    }
}
