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

        /// @brief Creates a new GLFW window
        /// @param w Width
        /// @param h Height
        /// @param title Window title
        /// @param api Rendering API (Vulkan, OpenGL, DirectX)
        BEwindow(int w, int h, std::string title, int api);
        ~BEwindow();

        BEwindow(const BEwindow&) = delete;
        BEwindow& operator=(const BEwindow&) = delete;

        /// @brief Checks if the window should close
        /// @return glfwWindowShouldClose
        bool shouldClose() { return glfwWindowShouldClose(window); }
        /// @brief Checks if the window was resized to call the resize callback.
        /// @return true if the window was resized, false otherwise
        bool wasWindowResized() { return frameBufferResized; }

        /// @brief Returns VkExtent2D of the window dimensions
        /// @return VkExtent2D of the window dimensions
        VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
        /// @brief Returns GLFWwindow pointer
        /// @return GLFWwindow pointer
        GLFWwindow* getWindow() { return window; }

        /// @brief Creates a Vulkan surface.
        /// @param instance Vulkan instance
        /// @param surface surface to bind to
        void createVkWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        /// @brief Resets window resized flag.
        void resetWindowResizedFlag() { frameBufferResized = false; }

        /// @brief Destroys the window and reinitalizes it with the new render API.
        /// @param api Render API to switch to (Vulkan, OpenGL, DirectX)
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