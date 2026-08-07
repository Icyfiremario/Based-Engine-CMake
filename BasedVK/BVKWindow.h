#ifndef BVKWINDOW_H
#define BVKWINDOW_H

// Vulkan
#include <vulkan/vulkan.h>

// Plog
#include <plog/Log.h>

// BasedCore
#include "../BasedCore/Common/BEWindow.h"


class BVKWindow final : BasedEngine::Common::BEWindow
{
public:

    BVKWindow();
    BVKWindow(int w, int h, const char* title);

    [[nodiscard]] bool shouldClose() const override { return glfwWindowShouldClose(m_window); }
    [[nodiscard]] bool wasWindowResized() const override { return frameBufferResized; }

    [[nodiscard]] VkExtent2D getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

    [[nodiscard]] GLFWwindow* getWindow() const override { return m_window; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;

private:

    GLFWwindow* m_window = nullptr;

    int width = 600, height = 600;
    int xPos = 0, yPos = 0; // Window position

    double cXPos = 0, cYPos = 0; // Cursor position
    int cursorMode = GLFW_CURSOR_NORMAL;

    std::string m_title = "Vulkan App";

    bool frameBufferResized = false;

    void initWindow() override;

    static void frameBufferResizedCallback(GLFWwindow* window, int w, int h);
};


#endif // BVKWINDOW_H