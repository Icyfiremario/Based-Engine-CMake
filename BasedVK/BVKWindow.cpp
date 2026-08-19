#include "BVKWindow.h"

BVKWindow::BVKWindow()
{
    initWindow();
}

BVKWindow::BVKWindow(const int w, const int h, const char* title) : BEWindow(w, h, title), width(w), height(h), m_title(title)
{
    initWindow();
}

void BVKWindow::createWindowSurface(const VkInstance instance, VkSurfaceKHR* surface) const
{
    if (const VkResult result = glfwCreateWindowSurface(instance, m_window, nullptr, surface); result != VK_SUCCESS)
    {
        PLOGF << "Failed to create window surface: " << result;
        throw std::runtime_error("Failed to create window surface!");
    }

    PLOGI << "Window surface created.";
}

void BVKWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, frameBufferResizedCallback);

    PLOGI << "Window created.";
}

void BVKWindow::frameBufferResizedCallback(GLFWwindow* window, const int w, const int h)
{
    const auto appWindow = static_cast<BVKWindow*>(glfwGetWindowUserPointer(window));

    appWindow->frameBufferResized = true;
    appWindow->width = w;
    appWindow->height = h;

    PLOGV << "Window resized.";
}
