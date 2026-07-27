#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 400, "Based Vulkan");
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);
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

        glfwPollEvents();
    }
}
