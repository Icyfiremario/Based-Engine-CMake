#include "BGLWindow.h"

BGLWindow::BGLWindow()
{
    BGLWindow::initWindow();
}

BGLWindow::BGLWindow(const int w, const int h, const char* title) : BEWindow(w, h, title), width(w), height(h), title(title)
{
    BGLWindow::initWindow();
}

void BGLWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(m_window, frameBufferResizedCallBack);

}

void BGLWindow::frameBufferResizedCallBack(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}
