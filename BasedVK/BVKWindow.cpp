#include "BVKWindow.h"

BVKWindow::BVKWindow(const int w, const int h, std::string title) : BEWindow(w, h, std::move(title)), width(w), height(h), m_title(std::move(title))
{
    initWindow();
}

void BVKWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
}

void BVKWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, frameBufferResizedCallback);
}

void BVKWindow::frameBufferResizedCallback(GLFWwindow* window, const int w, const int h)
{
    const auto appWindow = static_cast<BVKWindow*>(glfwGetWindowUserPointer(window));

    appWindow->frameBufferResized = true;
    appWindow->width = w;
    appWindow->height = h;
}
