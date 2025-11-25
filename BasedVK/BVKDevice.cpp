#include "BVKDevice.h"

// STD
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
#ifdef DEBUG
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
#endif // DEBUG

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    if (auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")); func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")); func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

BVKDevice::BVKDevice(BVKWindow &window) : window(window)
{
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createCommandPool();
}

BVKDevice::~BVKDevice()
{
    vkDestroyCommandPool(device_, commandPool, nullptr);
    vkDestroyDevice(device_, nullptr);

    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
}

uint32_t BVKDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
}

VkFormat BVKDevice::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
}

void BVKDevice::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer&buffer, VkDeviceMemory&bufferMemory)
{
}

VkCommandBuffer BVKDevice::beginSingleTimeCommands()
{
}

void BVKDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
}

void BVKDevice::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
}

void BVKDevice::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
{
}

void BVKDevice::createImageWidthInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties,
    VkImage&image, VkDeviceMemory&imageMemory)
{
}

void BVKDevice::createInstance()
{
}

void BVKDevice::setupDebugMessenger()
{
}

void BVKDevice::createSurface()
{
}

void BVKDevice::pickPhysicalDevice()
{
}

void BVKDevice::createLogicalDevice()
{
}

void BVKDevice::createCommandPool()
{
}

bool BVKDevice::isDeviceSuitable(VkPhysicalDevice device)
{
}

std::vector<const char *> BVKDevice::getRequiredExtensions()
{
}

bool BVKDevice::checkValidationLayerSupport()
{
}

QueueFamilyIndices BVKDevice::findQueueFamilies(VkPhysicalDevice device)
{
}

void BVKDevice::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
}

void BVKDevice::hasGlfwRequiredExtensions()
{
}

bool BVKDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
}

SwapChainSupportDetails BVKDevice::querySwapChainSupport(VkPhysicalDevice device)
{
}
