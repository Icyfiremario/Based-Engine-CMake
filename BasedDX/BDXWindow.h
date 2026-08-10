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
    BDXWindow(int w, int h, const std::string &title);

    [[nodiscard]] HWND getHWND() const { return m_hWnd; }
private:

    HWND m_hWnd;

    std::unique_ptr<BDXPipeline> m_pipeline;

    void initWindow() override;
};


#endif // BDXWINDOW_H