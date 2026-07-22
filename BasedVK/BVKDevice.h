#ifndef BVKDEVICE_H
#define BVKDEVICE_H

// STD
#include <vector>

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
    explicit BVKDevice(VkPhysicalDevice physicalDevice);
    ~BVKDevice() = default;

private:

    VkPhysicalDevice m_physicalDevice;
};


#endif // BVKDEVICE_H
