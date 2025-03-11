#pragma once

// STD
#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>

// GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Vulkan
#include <vulkan/vulkan.h>

// GLAD
#include <GL/glew.h>

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

        GLFWwindow* getWindow() { return window; }

        void resetWindowResizedFlag() { frameBufferResized = false; }

        void switchRenderAPI(int api);

    private:

        GLFWwindow* window;

        int width, height, renderAPI;
        std::string windowName;

        bool frameBufferResized = false;

        void initWindow();

        static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
        
};