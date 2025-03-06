#include "BEwinddow.h"

BEwindow::BEwindow(int w, int h, std::string title) : width(w), height(h), windowName(title)
{
    initWindow();
}

BEwindow::~BEwindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void BEwindow::createVkWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create VK window surface!");
    }
}

void BEwindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

void BEwindow::frameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto appWindow = reinterpret_cast<BEwindow*>(glfwGetWindowUserPointer(window));

    appWindow->frameBufferResized = true;
    appWindow->width = width;
    appWindow->height = height;
}
