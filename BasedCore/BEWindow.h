#ifndef BEWINDOW_H
#define BEWINDOW_H

// STD
#include <memory>
#include <stdexcept>

#ifdef WIN32
#include <windows.h>
#include <directx/d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directx/d3dx12.h>
#endif

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Vulkan
#include <vulkan/vulkan.hpp>

class BEWindow
{
public:
    BEWindow(int w, int h, std::string title);
    virtual ~BEWindow() = default;


private:

    GLFWwindow* m_window;

    int width, height;
    int xPos, yPos; // Window position

    double cXPos, cYPos; // Cursor position
    int cursorMode = GLFW_CURSOR_NORMAL;

    std::string m_title;

    bool frameBufferResized = false;

    /// @brief Checks if the window is valid then calls frameBufferResizedFunc();
    static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
    /// @brief Resizes the buffer
    virtual void frameBufferResizedFunc(int width, int height);
};


#endif // BEWINDOW_H