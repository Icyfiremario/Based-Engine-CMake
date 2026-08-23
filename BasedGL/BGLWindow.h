#ifndef BGLWINDOW_H
#define BGLWINDOW_H

// BasedCore
#include "../BasedCore/Common/BEWindow.h"

class BGLWindow : BasedEngine::Common::BEWindow
{
public:

    BGLWindow();
    explicit BGLWindow(int w, int h, const char* title);


private:

    GLFWwindow* m_window = nullptr;

    int width = 600, height = 600;
    std::string title = "OpenGL App";

    void initWindow() override;

    static void frameBufferResizedCallBack(GLFWwindow* window, int width, int height);
};


#endif // BGLWINDOW_H