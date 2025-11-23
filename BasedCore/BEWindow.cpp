#include "BEWindow.h"

BEWindow::BEWindow(int w, int h, std::string title)
{
}

void BEWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
{
    void* windowPtr = glfwGetWindowUserPointer(window);
    auto instance = static_cast<BEWindow*>(windowPtr);

    if (instance)
    {
        instance->frameBufferResizedFunc(width, height);
    }
}

void BEWindow::frameBufferResizedFunc(int width, int height)
{
    throw std::logic_error("No API in use!");
}
