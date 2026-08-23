#include "BGLApp.h"

BGLApp::BGLApp()
{
    appWindow = std::make_unique<BGLWindow>(600, 400, "OpenGL App");
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
}
