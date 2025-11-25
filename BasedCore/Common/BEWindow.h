#ifndef BEWINDOW_H
#define BEWINDOW_H

// STD
#include <memory>
#include <stdexcept>
#include <string>

// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace BasedEngine::Common
{
    class BEWindow
    {
    public:
        BEWindow(int w, int h, std::string title);
        virtual ~BEWindow();


    private:

        GLFWwindow* m_window = nullptr;

        int width, height;
        int xPos, yPos; // Window position

        double cXPos, cYPos; // Cursor position
        int cursorMode = GLFW_CURSOR_NORMAL;

        std::string m_title;

        bool frameBufferResized = false;

        virtual void initWindow();

        /// @brief Checks if the window is valid then calls frameBufferResizedFunc();
        static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
        /// @brief Resizes the buffer
        virtual void frameBufferResizedFunc(int width, int height);
    };
}



#endif // BEWINDOW_H