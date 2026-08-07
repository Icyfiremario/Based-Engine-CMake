#include "BVKDevice.h"

BVKDevice::BVKDevice(const VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) : m_physicalDevice(physicalDevice), surface_(surface)
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

uint32_t BVKDevice::findMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    PLOGF << "Failed to find a suitable memory type.";
    throw std::runtime_error("Failed to find a suitable memory type.");
}

VkFormat BVKDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const
{
    for (const VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    PLOGF << "Failed to find a suitable format.";
    throw std::runtime_error("Failed to find a suitable format.");
}

void BVKDevice::createBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device_, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        PLOGF << "Failed to create buffer.";
        throw std::runtime_error("Failed to create VkBuffer.");
    }

    PLOGI << "VkBuffer created.";

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device_, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device_, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        PLOGF << "Failed to allocate buffer memory.";
        throw std::runtime_error("Failed to allocate buffer memory.");
    }

    vkBindBufferMemory(device_, buffer, bufferMemory, 0);

    PLOGI << "VkBuffer allocated";
}

VkCommandBuffer BVKDevice::beginSingleTimeCommands() const
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}

void BVKDevice::endSingleTimeCommands(const VkCommandBuffer commandBuffer) const
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue_);

    vkFreeCommandBuffers(device_, commandPool, 1, &commandBuffer);
}

void BVKDevice::copyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize size) const
{
    const VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};

    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void BVKDevice::copyBufferToImage(const VkBuffer buffer, const VkImage image, const uint32_t width, const uint32_t height, const uint32_t layerCount) const
{
    const VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = layerCount;

    region.imageOffset = {.x = 0,.y = 0, .z = 0};
    region.imageExtent = {.width = width, .height = height, .depth = 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
}

void BVKDevice::createImageWidthInfo(const VkImageCreateInfo& imageInfo, const VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const
{
    if (vkCreateImage(device_, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        PLOGF << "Failed to create image.";
        throw std::runtime_error("Failed to create image!");
    }

    VkMemoryRequirements memRequirements{};
    vkGetImageMemoryRequirements(device_, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device_, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        PLOGF << "Failed to allocate image memory.";
        throw std::runtime_error("Failed to allocate image memory!");
    }

    if (vkBindImageMemory(device_, image, imageMemory, 0) != VK_SUCCESS)
    {
        PLOGF << "Failed to bind image memory.";
        throw std::runtime_error("Failed to bind image memory!");
    }
}

bool BVKDevice::isSuitable() const
{
    const QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
    const bool extensionsSupported = checkDeviceExtensionSupport(m_physicalDevice);

    bool swapChainAdequate = false;

    if (indices.isComplete() && extensionsSupported)
    {
        const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void BVKDevice::createLogicalDevice()
{
    const QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    if (!indices.isComplete())
    {
        PLOGF << "Queue families are incomplete. Cannot create logical device.";
        throw std::runtime_error("Failed to find required queue families!");
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const std::set uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

    constexpr float queuePriority = 1.0f;
    for (const uint32_t queueFamily : uniqueQueueFamilies)
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
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &device_) != VK_SUCCESS)
    {
        PLOGF << "Failed to create logical device: ";
        throw std::runtime_error("Failed to create logical device!");
    }

    PLOGI << "Logical device created";

    vkGetDeviceQueue(device_, indices.graphicsFamily, 0, &graphicsQueue_);
    vkGetDeviceQueue(device_, indices.presentFamily, 0, &presentQueue_);

    PLOGI << "Device queues collected";
}

void BVKDevice::createCommandPool()
{
    const QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        PLOGF << "Failed to create command pool.";
        throw std::runtime_error("Failed to create command pool!");
    }

    PLOGI << "Command pool created.";
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
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface_, &presentSupport);

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
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface_, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, details.presentModes.data());
    }

    return details;
}
