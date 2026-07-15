#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 600, "Vulkan App");

    BVKDeviceManager* deviceManager = BVKDeviceManager::getInstance();
}

BVKApp::~BVKApp()
{

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
