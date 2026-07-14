#include "BEWindow.h"


BasedEngine::Common::BEWindow::BEWindow() : width(600), height(600)
{
}

BasedEngine::Common::BEWindow::BEWindow(const int w, const int h, std::string title) : width(w), height(h), m_title(std::move(title))
{
}

BasedEngine::Common::BEWindow::~BEWindow()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
