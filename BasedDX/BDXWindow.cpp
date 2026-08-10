#include "BDXWindow.h"

BDXWindow::BDXWindow(int w, int h, const std::string& title) : BEWindow(w, h, title)
{
    m_pipeline = std::make_unique<BDXPipeline>();
}

void BDXWindow::initWindow()
{
}
