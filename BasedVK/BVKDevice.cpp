#include "BVKDevice.h"

BVKDevice::BVKDevice(const VkPhysicalDevice physicalDevice, VkSurfaceKHR* surface) : m_physicalDevice(physicalDevice), surface_(surface)
{
    PLOGI << "Vulkan device created";

    createLogicalDevice();
    createCommandPool();
}

BVKDevice::~BVKDevice()
{
    vkDestroyCommandPool(device_, commandPool, nullptr);
    vkDestroyDevice(device_, nullptr);

    PLOGI << "Vulkan device released";
}

bool BVKDevice::isSuitable() const
{
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
    bool extensionsSupported = checkDeviceExtensionSupport(m_physicalDevice);

    bool swapChainAdequate = false;

    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void BVKDevice::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers)
    {
        //createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    }
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

SwapChainSupportDetails BVKDevice::querySwapChainSupport(const VkPhysicalDevice physicalDevice) const
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, *surface_, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, *surface_, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, *surface_, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, *surface_, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, *surface_, &presentModeCount, details.presentModes.data());
    }

    return details;
}
