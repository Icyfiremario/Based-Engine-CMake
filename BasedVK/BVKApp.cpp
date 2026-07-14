#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(600, 600, "Vulkan App");
}

BVKApp::~BVKApp()
{

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
