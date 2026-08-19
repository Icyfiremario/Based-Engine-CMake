#ifndef BDXWINDOW_H
#define BDXWINDOW_H

// BasedCore
#include "../BasedCore/Common/BEWindow.h"

// BasedDX
#include "stdafx.h"
#include "BDXPipeline.h"

class BDXWindow final : BasedEngine::Common::BEWindow
{
public:

    BDXWindow();
    BDXWindow(int w, int h, const std::string &title);

    [[nodiscard]] bool shouldClose() const override { return glfwWindowShouldClose(m_glfwWindow); }

    [[nodiscard]] GLFWwindow* getWindow() const override { return m_glfwWindow; };

    [[nodiscard]] HWND getHWND() const { return m_hWnd; }
private:

    HWND m_hWnd;
    GLFWwindow* m_glfwWindow = nullptr;

    int width = 600, height = 600;

    std::string title = "DirectX App";

    std::unique_ptr<BDXPipeline> m_pipeline;

    void initWindow() override;
};


#endif // BDXWINDOW_H