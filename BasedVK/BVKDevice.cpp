#include "BVKDevice.h"

// STD
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
#ifndef NDEBUG
	std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
#endif // !NDEBUG

	return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
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
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

BVKDevice::BVKDevice(BEwindow &window) : window{window}
{
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createCommandsPool();
}

BVKDevice::~BVKDevice()
{
}

uint32_t BVKDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    return 0;
}

VkFormat BVKDevice::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    return VkFormat();
}

void BVKDevice::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
}

VkCommandBuffer BVKDevice::beginSingleTimeCommands()
{
    return VkCommandBuffer();
}

void BVKDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
}

void BVKDevice::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
}

void BVKDevice::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
}

void BVKDevice::createImageWidthInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
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
    if (glfwCreateWindowSurface(instance, window.getWindow(), nullptr, &surface_) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void BVKDevice::pickPhysicalDevice()
{
}

void BVKDevice::createLogicalDevice()
{
}

void BVKDevice::createCommandsPool()
{
}

bool BVKDevice::isDeviceSuitable(VkPhysicalDevice device)
{
    return false;
}

std::vector<const char *> BVKDevice::getRequiredExtensions()
{
    return std::vector<const char *>();
}

bool BVKDevice::checkValidationLayerSupport()
{
    return false;
}

QueueFamilyIndicies BVKDevice::findQueueFamilies(VkPhysicalDevice device)
{
    return QueueFamilyIndicies();
}

void BVKDevice::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
}

void BVKDevice::hasGflwRequiredInstanceExtensions()
{
}

bool BVKDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    return false;
}

SwapChainSupportDetails BVKDevice::querySwapChainSupport(VkPhysicalDevice device)
{
    return SwapChainSupportDetails();
}
