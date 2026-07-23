#ifndef BVKDEVICE_H
#define BVKDEVICE_H

// STD
#include <vector>
#include <set>
#include <set>

// Vulkan
#include <vulkan/vulkan.h>

// PLog
#include <plog/Log.h>

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
    uint32_t graphicsFamily{};
    uint32_t presentFamily{};

    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;

    [[nodiscard]] bool isComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class BVKDevice
{
public:
    explicit BVKDevice(VkPhysicalDevice physicalDevice, BVKWindow* window, VkSurfaceKHR* surface);
    ~BVKDevice();

    [[nodiscard]] VkSurfaceKHR* getSurface() const { return surface_; }

    bool isSuitable();

private:

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

    BVKWindow* m_window = nullptr;

    VkDevice device_;
    VkSurfaceKHR* surface_ = nullptr;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;

    const std::vector <const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void createLogicalDevice();
    void createCommandPool();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) const;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) const;
};


#endif // BVKDEVICE_H
