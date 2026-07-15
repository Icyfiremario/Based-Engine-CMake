#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 400, "Based Vulkan");
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);
}

BVKApp::~BVKApp()
{
    appDevice.reset();
    appWindow.reset();
}

void BVKApp::run()
{
    while (!appWindow.get()->shouldClose())
    {
        if (glfwGetKey(appWindow.get()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(appWindow.get()->getWindow(), true);
        }

        glfwPollEvents();
    }
}
