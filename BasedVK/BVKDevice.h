#ifndef BVKDEVICE_H
#define BVKDEVICE_H

// STD
#include <vector>
#include <string>

// BasedCore
#include "../BasedCore/Common/BEDevice.h"

// BasedVK
#include "BVKWindow.h"

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    uint32_t graphicsFamily;
    uint32_t presentFamily;

    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;

    [[nodiscard]] bool isComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class BVKDevice : BasedEngine::Common::BEDevice
{
public:
#ifdef DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif

    VkPhysicalDeviceProperties properties{};

    explicit BVKDevice(BVKWindow& window);
    ~BVKDevice();

    BVKDevice(const BVKDevice&) = delete;
    BVKDevice &operator=(const BVKDevice&) = delete;
    BVKDevice(BVKDevice&&) = delete;
    BVKDevice &operator=(BVKDevice&&) = delete;

    [[nodiscard]] VkCommandPool getCommandPool() const { return commandPool;}
    [[nodiscard]] VkDevice getDevice() const { return device_; }
    [[nodiscard]] VkSurfaceKHR getSurface() const { return surface_; }
    [[nodiscard]] VkQueue getGraphicsQueue() const { return graphicsQueue_; }
    [[nodiscard]] VkQueue getPresentQueue() const { return presentQueue_; }

    SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
    VkCommandBuffer beginSingleTimeCommands() const;
    void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) const;

    void createImageWidthInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) const;

private:
#ifdef DEBUG
    const bool logDevice = true;
#else
    const bool logDevice = false;
#endif

    VkInstance instance{};
    VkDebugUtilsMessengerEXT debugMessenger{};
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkCommandPool commandPool{};

    BVKWindow& window;

    VkDevice device_{};
    VkSurfaceKHR surface_{};
    VkQueue graphicsQueue_{};
    VkQueue presentQueue_{};

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();

    bool isDeviceSuitable(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void hasGlfwRequiredExtensions();
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
};

#endif // BVKDEVICE_H