#pragma once

// STD
#include <vector>
#include <string>

// BasedCore
#include "../BasedCore/BEwindow.h"

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndicies
{
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class BVKDevice
{
    public:

#ifdef DEBUG
        const bool enableValidationLayers = true;
#else
        const bool enableValidationLayers = false;
#endif //DEBUG

        VkPhysicalDeviceProperties properties;

        BVKDevice(BEwindow &window);
        ~BVKDevice();

        BVKDevice(const BVKDevice &) = delete;
        BVKDevice &operator=(const BVKDevice &) = delete;
        BVKDevice(BVKDevice &&) = delete;
        BVKDevice &operator=(BVKDevice &&) = delete;

        VkCommandPool getCommandPool() { return commandPool; }
        VkDevice getDevice() { return device_; }
        VkSurfaceKHR getSurface() { return surface_; }
        VkQueue getGraphicsQueue() { return graphicsQueue_; }
        VkQueue getPresentQueue() { return presentQueue_; }

        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndicies findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

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