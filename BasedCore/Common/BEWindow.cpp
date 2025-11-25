#include "BEWindow.h"

#include <utility>

BasedEngine::Common::BEWindow::BEWindow(const int w, const int h, std::string title) : width(w), height(h), m_title(std::move(title))
{
    BEWindow::initWindow();
}

BasedEngine::Common::BEWindow::~BEWindow()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void BasedEngine::Common::BEWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(width, height, m_title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, frameBufferResizedCallback);
}

void BasedEngine::Common::BEWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
{
    void* windowPtr = glfwGetWindowUserPointer(window);

    if (const auto instance = static_cast<BEWindow*>(windowPtr))
    {
        instance->frameBufferResizedFunc(width, height);
    }
}

void BasedEngine::Common::BEWindow::frameBufferResizedFunc(int width, int height)
{
    throw std::logic_error("No API in use!");
}
