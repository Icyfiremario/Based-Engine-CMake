#include "BVKDevice.h"

BVKDevice::BVKDevice(const VkPhysicalDevice physicalDevice, BVKWindow* window, VkSurfaceKHR* surface) : m_physicalDevice(physicalDevice), m_window(window), surface_(surface)
{
    PLOGI << "Vulkan device created";
}

BVKDevice::~BVKDevice()
{
    PLOGI << "Vulkan device released";
}

bool BVKDevice::isSuitable()
{
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
    bool extensionsSupported = checkDeviceExtensionSupport(m_physicalDevice);

    bool swapChainAdequate = false;

    if (extensionsSupported)
    {

    }
}

void BVKDevice::createLogicalDevice()
{
}

void BVKDevice::createCommandPool()
{
}

QueueFamilyIndices BVKDevice::findQueueFamilies(const VkPhysicalDevice physicalDevice) const
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
            indices.graphicsFamilyHasValue = true;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, *surface_, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = i;
            indices.presentFamilyHasValue = true;
        }

        if (indices.isComplete())
        {
            break;
        }
    }

    return indices;
}

bool BVKDevice::checkDeviceExtensionSupport(const VkPhysicalDevice physicalDevice) const
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : extensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails BVKDevice::querySwapChainSupport(VkPhysicalDevice physicalDevice) const
{
}
