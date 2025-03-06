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

        /// @brief Creates GLFW window
        /// @param w width
        /// @param h height
        /// @param title title
        BEwindow(int w, int h, std::string title);
        ~BEwindow();

        /// @brief Returns whether or not the GLFW window should close
        /// @return glfwWindowShouldClose
        bool shouldClose() { return glfwWindowShouldClose(window); }

        /// @brief Checks if the window was resized
        /// @return frameBufferResized
        bool wasWindowREsized() { return frameBufferResized; }

        /// @brief Returns the window's width and height as VkExtent2D
        /// @return {width, height}
        VkExtent2D getVKExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        /// @brief 
        /// @param w pointer to varible store the width
        /// @param h pointer to varible store the height
        void getGLExtent(int* w, int* h) { glfwGetFramebufferSize(window, w, h); }
        
        /// @brief returns the GLFWwindow pointer
        /// @return window
        GLFWwindow* getWindow() { return window; }

        /// @brief creates window surface for vulkan
        /// @param instance Vulkan instance
        /// @param surface Vulkan KHR surface
        void createVkWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        /// @brief NOT IMPLEMENTED
        void createGlWindowSurface();

        /// @brief resets the window resized flag
        void resetWindowResizedFlag() { frameBufferResized = false; }

    private:

        GLFWwindow* window;

        int width, height;

        bool frameBufferResized = false;

        std::string windowName;

        void initWindow();

        static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
};