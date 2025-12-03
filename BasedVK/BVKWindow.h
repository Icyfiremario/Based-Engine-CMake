#ifndef BVKWINDOW_H
#define BVKWINDOW_H

// Vulkan
#include <vulkan/vulkan.hpp>

// BasedCore
#include "../BasedCore/Common/BEWindow.h"


class BVKWindow final : BasedEngine::Common::BEWindow
{
public:

    BVKWindow(int w, int h, std::string title);

    bool shouldClose() const override { return glfwWindowShouldClose(m_window); }
    bool wasWindowResized() const override { return frameBufferResized; }

    GLFWwindow* getWindow() const override { return m_window; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private:

    GLFWwindow* m_window = nullptr;

    int width{}, height{};
    int xPos = 0, yPos = 0; // Window position

    double cXPos = 0, cYPos = 0; // Cursor position
    int cursorMode = GLFW_CURSOR_NORMAL;

    std::string m_title;

    bool frameBufferResized = false;

    void initWindow() override;

    static void frameBufferResizedCallback(GLFWwindow* window, int w, int h);
};


#endif // BVKWINDOW_H