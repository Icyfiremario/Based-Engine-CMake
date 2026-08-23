#include "BGLApp.h"

BGLApp::BGLApp()
{
    appWindow = std::make_unique<BGLWindow>(800, 600, "OpenGL App");
}

BGLApp::BGLApp(int w, int h, const char* title)
{
    appWindow = std::make_unique<BGLWindow>(w, h, title);
}

BGLApp::~BGLApp()
{
    appWindow.reset();
}

void BGLApp::run()
{
    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            PLOGI << "Escape key pressed. Closing app...";
            glfwSetWindowShouldClose(appWindow->getWindow(), GLFW_TRUE);
        }

        glfwPollEvents();
    }
}
