#pragma once

// STD
#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>

// GLEW
#include <GL/glew.h>
#include <GL/glut.h>

// Vulkan 
#include <vulkan/vulkan.h>


// GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/// @brief GLFW window wrapper class
class BEwindow
{
    public:

        BEwindow(int w, int h, std::string title);
        ~BEwindow();

        bool shouldClose() { return glfwWindowShouldClose(window); }
        bool wasWindowREsized() { return frameBufferResized; }

        VkExtent2D getVKExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        void getGLExtent(int* w, int* h) { glfwGetFramebufferSize(window, w, h); }
        GLFWwindow* getWindow() { return window; }

        void createVkWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        void createGlWindowSurface();

        void resetWindowResizedFlag() { frameBufferResized = false; }

    private:

        GLFWwindow* window;

        int width, height;

        bool frameBufferResized = false;

        std::string windowName;

        void initWindow();

        static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
};