#pragma once

// STD
#include <vector>
#include <string>

// BasedCore
#include "../BasedCore/BEwindow.h"

/// @brief Vulkan swap chain support info
struct SwapChainSupportDetails
{
    /// @brief Surface capabilities
    VkSurfaceCapabilitiesKHR capabilities;
    /// @brief Available formats
    std::vector<VkSurfaceFormatKHR> formats;
    /// @brief Available present modes
    std::vector<VkPresentModeKHR> presentModes;
};

/// @brief Queue family index information
struct QueueFamilyIndicies
{
    /// @brief Graphics queue family index
    uint32_t graphicsFamily;
    /// @brief Preset queue family index
    uint32_t presentFamily;
    /// @brief If we have a graphics queue family
    bool graphicsFamilyHasValue = false;
    /// @brief If we have a present queue family
    bool presentFamilyHasValue = false;
    /// @brief Checks if we have a graphics queue family and a present queue family
    /// @return graphicsFamilyHasValue && presentFamilyHasValue
    bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

/// @brief Vulkan device
class BVKDevice
{
    public:

#ifdef DEBUG
        const bool enableValidationLayers = true;
#else
        /// @brief Enable validation layers for debugging
        const bool enableValidationLayers = false;
#endif //DEBUG

        /// @brief Physical device properties
        VkPhysicalDeviceProperties properties;

        /// @brief Creates and sets up device and binds it to the GLFW window
        /// @param window 
        BVKDevice(BEwindow &window);
        ~BVKDevice();

        BVKDevice(const BVKDevice &) = delete;
        BVKDevice &operator=(const BVKDevice &) = delete;
        BVKDevice(BVKDevice &&) = delete;
        BVKDevice &operator=(BVKDevice &&) = delete;

        /// @brief Gets command pool
        /// @return Command pool
        VkCommandPool getCommandPool() { return commandPool; }
        /// @brief Gets device
        /// @return Device
        VkDevice getDevice() { return device_; }
        /// @brief Gets surface
        /// @return Surface
        VkSurfaceKHR getSurface() { return surface_; }
        /// @brief Gets graphics queue
        /// @return Graphics queue
        VkQueue getGraphicsQueue() { return graphicsQueue_; }
        /// @brief Gets present queue
        /// @return present queue
        VkQueue getPresentQueue() { return presentQueue_; }

        /// @brief Querys for swap chain support of the physical device
        /// @return Swap chain support of the physical
        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
        /// @brief Finds supported memory types of the vulkan device
        /// @param typeFilter Memory types to check for
        /// @param properties Memory properties
        /// @return Supported memory types
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        /// @brief Finds queue families of the physical device
        /// @return Queue family indicies
        QueueFamilyIndicies findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
        /// @brief Finds the vulkan device's supported surface formats
        /// @param candidates List of potential surface formats
        /// @param tiling Tiling method to use
        /// @param features Features to use
        /// @return Vulkan Format
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        /// @brief Creates a buffer on the vulkan device
        /// @param size Buffer size
        /// @param usage Usage flags
        /// @param properties Properties flags
        /// @param buffer Buffer to write to
        /// @param bufferMemory Memory to allocate
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
        /// @brief Enables writing single time commands to a command buffer
        /// @return Command buffer to write commands to
        VkCommandBuffer beginSingleTimeCommands();
        /// @brief Disables writing single timecommands to a command buffer
        /// @param commandBuffer Command buffer to stop commands for.
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        /// @brief Copies the contents of one buffer to another
        /// @param srcBuffer Buffer to copy from
        /// @param dstBuffer Buffer to copy to
        /// @param size Size of the buffers
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        /// @brief Copies the contents of a buffer to an image object
        /// @param buffer Buffer to copy from
        /// @param image Image to copy to
        /// @param width Image width
        /// @param height Image height
        /// @param layerCount Color channel count
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

        /// @brief Creates image width information
        /// @param imageInfo Image create info
        /// @param properties Memory properties
        /// @param image Image object to fill
        /// @param imageMemory Image memory to fill
        void createImageWidthInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

    private:

#ifdef DEBUG
        const bool logDevice = true;
#else
        const bool logDevice = false;
#endif //DEBUG

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkCommandPool commandPool;

        BEwindow &window;

        VkDevice device_;
        VkSurfaceKHR surface_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;

        const std::vector <const char *> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        const std::vector <const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandsPool();

        bool isDeviceSuitable(VkPhysicalDevice device);
        std::vector<const char *> getRequiredExtensions();
        bool checkValidationLayerSupport();
        QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void hasGflwRequiredInstanceExtensions();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

};