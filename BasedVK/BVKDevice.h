#ifndef BVKDEVICE_H
#define BVKDEVICE_H

// STD
#include <vector>
#include <set>

// Vulkan
#include <vulkan/vulkan.h>

// PLog
#include <plog/Log.h>

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    uint32_t graphicsFamily{};
    uint32_t presentFamily{};

    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;

    [[nodiscard]] bool isComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class BVKDevice
{
public:

#ifdef DEBUG
    bool enableValidationLayers = true;
#else
    bool enableValidationLayers = false;
#endif

    explicit BVKDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    ~BVKDevice();

    [[nodiscard]] VkCommandPool getCommandPool() const { return commandPool; }

    [[nodiscard]] VkDevice getDevice() const { return device_; }
    [[nodiscard]] VkSurfaceKHR getSurface() const { return surface_; }
    [[nodiscard]] VkQueue getGraphicsQueue() const { return graphicsQueue_; }
    [[nodiscard]] VkQueue getPresentQueue() const { return presentQueue_; }

    [[nodiscard]] SwapChainSupportDetails getSwapChainSupport() const { return querySwapChainSupport(m_physicalDevice); }

    [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    [[nodiscard]] QueueFamilyIndices findPhysicalQueueFamilies() const { return findQueueFamilies(m_physicalDevice); }

    [[nodiscard]] VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const;

    [[nodiscard]] VkCommandBuffer beginSingleTimeCommands() const;
    void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) const;

    void createImageWidthInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) const;

    [[nodiscard]] bool isSuitable() const;

private:

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkCommandPool commandPool;

    VkDevice device_;
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    const std::vector <const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
#ifdef APPLE
        , "VK_KHR_portability_subset"
#endif
    };

    void createLogicalDevice();
    void createCommandPool();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) const;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) const;
};


#endif // BVKDEVICE_H
