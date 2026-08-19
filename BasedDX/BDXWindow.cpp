#include "BDXWindow.h"

BDXWindow::BDXWindow()
{
    initWindow();
    m_pipeline = std::make_unique<BDXPipeline>();
}

BDXWindow::BDXWindow(const int w, const int h, const std::string& title) : BEWindow(w, h, title), width(w), height(h), title(title)
{
    initWindow();
    m_pipeline = std::make_unique<BDXPipeline>();
}

void BDXWindow::initWindow()
{
    if (!glfwInit())
    {
        PLOGF << "Failed to initialize GLFW.";
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_glfwWindow, this);

    m_hWnd = glfwGetWin32Window(m_glfwWindow);

    PLOGI << "Window created.";
}
