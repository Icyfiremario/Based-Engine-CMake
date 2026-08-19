#include "BDXApp.h"

BDXApp::BDXApp()
{
    appWindow = std::make_unique<BDXWindow>(800, 600, "Based DirectX");
}

BDXApp::BDXApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BDXWindow>(width, height, title);
}

BDXApp::~BDXApp()
{
    appWindow.reset();
}

void BDXApp::run()
{
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
