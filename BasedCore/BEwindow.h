#pragma once

// STD
#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>

// GLAD
#include <GL/glew.h>

// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Vulkan
#include <vulkan/vulkan.h>

// BasedCore
#include "BEenums.h"

/// @brief GLFW window wrapper class
class BEwindow
{
    public:

        BEwindow(int w, int h, std::string title, int api);
        ~BEwindow();

        BEwindow(const BEwindow&) = delete;
        BEwindow& operator=(const BEwindow&) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        bool wasWindowResized() { return frameBufferResized; }

        VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
        GLFWwindow* getWindow() { return window; }

        void createVkWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        void resetWindowResizedFlag() { frameBufferResized = false; }

        void switchRenderAPI(int api);

    private:

        GLFWwindow* window;

        int width, height, renderAPI;
        std::string windowName;

        bool frameBufferResized = false;

        void initWindow();

        static void vkFrameBufferResizeCallback(GLFWwindow* window, int width, int height);
        static void glFrameBufferResizeCallback(GLFWwindow* window, int width, int height);
        
};