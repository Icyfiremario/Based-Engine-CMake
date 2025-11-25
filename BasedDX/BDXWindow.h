#ifndef BDXWINDOW_H
#define BDXWINDOW_H

// Windows
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <directxmath.h>

// BasedCore
#include "../BasedCore/Common/BEWindow.h"

class BDXWindow final : BasedEngine::Common::BEWindow
{
public:
    BDXWindow(int w, int h, const std::string &title);

    [[nodiscard]] HWND getHWND() const { return m_hWnd; }
private:

    HWND m_hWnd;
};


#endif // BDXWINDOW_H