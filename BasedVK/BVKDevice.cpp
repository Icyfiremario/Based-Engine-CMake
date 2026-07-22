#include "BVKDevice.h"

BVKDevice::BVKDevice(const VkPhysicalDevice physicalDevice) : m_physicalDevice(physicalDevice)
{
    PLOGI << "Vulkan device created";
}
