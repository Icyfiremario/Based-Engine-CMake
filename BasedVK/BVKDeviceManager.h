#ifndef BVKDEVICEMANAGER_H
#define BVKDEVICEMANAGER_H

// STD
#include <memory>
#include <mutex>
#include <iostream>
#include <vector>
#include <cstring>

// Vulkan
#include <vulkan/vulkan.h>

// GLFW
#include <GLFW/glfw3.h>

// Plog
#include <plog/Log.h>

// BasedVK
#include "BVKDevice.h"

#ifndef FLAGS
#ifdef APPLE
#define FLAGS VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR
#else
#define FLAGS 0
#endif // APPLE
#endif // FLAGS

/// @brief Manages a list of all available vulkan ready devices suitable for the application.
class BVKDeviceManager
{
private:
#ifdef DEBUG
    const bool logDevice = true;
#else
    const bool logDevice = false;
#endif

    struct Token {};

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::vector<std::unique_ptr<BVKDevice>> devices;
    int currentDeviceIndex = 0; /// @brief Index of current device in device list.

    BVKDeviceManager();

    void createInstance();
    void setupDebugMessenger();
    void findDevices();

    [[nodiscard]] std::vector<const char*> getRequiredExtensions() const;
    [[nodiscard]] bool checkValidationLayerSupport() const;

    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

public:

#ifdef DEBUG
    bool enableValidationLayers = true;
#else
    bool enableValidationLayers = false;
#endif

    explicit BVKDeviceManager(Token);

    BVKDeviceManager(const BVKDeviceManager&) = delete;
    BVKDeviceManager& operator=(const BVKDeviceManager&) = delete;
    BVKDeviceManager(BVKDeviceManager&&) = delete;
    BVKDeviceManager& operator=(BVKDeviceManager&&) = delete;

    ~BVKDeviceManager();

    static std::shared_ptr<BVKDeviceManager> getInstance();

    [[nodiscard]] VkInstance getVkInstance() const { return instance; };
    std::unique_ptr<BVKDevice> getDevicePtr();
};


#endif // BVKDEVICEMANAGER_H
