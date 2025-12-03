#ifndef BVKDEVICEMANAGER_H
#define BVKDEVICEMANAGER_H

// STD
#include <mutex>
#include <map>
#include <vector>
#include <iostream>

// BasedVK
#include "BVKDevice.h"

class BVKDeviceManager
{
public:

#ifdef DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif

    BVKDeviceManager(const BVKDeviceManager&) = delete;

    static BVKDeviceManager* getInstance();

    VkInstance getVkInstance() const { return vkInstance; }

private:

#ifdef DEBUG
    const bool logDevice = true;
#else
    const bool logDevice = false;
#endif

    static BVKDeviceManager* instance;
    static std::mutex mtx;

    VkInstance vkInstance;

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    using id_t = unsigned int;
    std::vector<BVKDevice*> availableDevices;

    BVKDeviceManager();
    ~BVKDeviceManager();

    void createInstance();
    void findDevices();
    bool isDeviceSuitable(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions() const;
    bool checkValidationLayerSupport() const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
};


#endif // BVKDEVICEMANAGER_H