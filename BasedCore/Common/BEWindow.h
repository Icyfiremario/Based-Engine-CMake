#ifndef BEWINDOW_H
#define BEWINDOW_H

// STD
#include <memory>
#include <stdexcept>
#include <string>

// GLFW
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace BasedEngine::Common
{
    class BEWindow
    {
    public:

        BEWindow(int w, int h, std::string title);
        virtual ~BEWindow();

        BEWindow(const BEWindow&) = delete;
        BEWindow& operator=(const BEWindow&) = delete;

        virtual bool shouldClose() const { return glfwWindowShouldClose(m_window); }
        virtual bool wasWindowResized() const { return frameBufferResized; }

        virtual GLFWwindow* getWindow() const { return m_window; }

    private:

        GLFWwindow* m_window = nullptr;

        int width, height;
        int xPos = 0, yPos = 0; // Window position

        double cXPos = 0, cYPos = 0; // Cursor position
        int cursorMode = GLFW_CURSOR_NORMAL;

        std::string m_title;

        bool frameBufferResized = false;

        virtual void initWindow() = 0;
    };
}
#endif // BEWINDOW_H