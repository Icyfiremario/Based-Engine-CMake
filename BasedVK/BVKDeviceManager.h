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
#include "BVKWindow.h"

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
#ifdef DEBUG
    const bool logDevice = true;
#else
    const bool logDevice = false;
#endif

    struct Token {};

    VkInstance instance{};
    VkDebugUtilsMessengerEXT debugMessenger{};

    BVKWindow* m_window = nullptr;

    VkSurfaceKHR surface_;

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::vector<std::shared_ptr<BVKDevice>> devices;
    int currentDeviceIndex = 0; /// @brief Index of current device in device list.

    BVKDeviceManager();
    explicit BVKDeviceManager(BVKWindow* window);

    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void findDevices();

    bool isDeviceSuitable(VkPhysicalDevice device);
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
    explicit BVKDeviceManager(Token, BVKWindow* window);

    BVKDeviceManager(const BVKDeviceManager&) = delete;
    BVKDeviceManager& operator=(const BVKDeviceManager&) = delete;
    BVKDeviceManager(BVKDeviceManager&&) = delete;
    BVKDeviceManager& operator=(BVKDeviceManager&&) = delete;

    ~BVKDeviceManager();

    /// @brief Returns a pointer to the class instance.
    static std::shared_ptr<BVKDeviceManager> getInstance();
    static std::shared_ptr<BVKDeviceManager> getInstance(BVKWindow* window);

    /// @brief Returns the Vulkan instance.
    [[nodiscard]] VkInstance getVkInstance() const { return instance; };
    /// @brief Returns a pointer to the current selected device. Default: 0
    std::shared_ptr<BVKDevice> getDevicePtr();
    /// @brief  Returns a pointer to the list of BVKDevice objects created by the manager for each found GPU.
    std::vector<std::shared_ptr<BVKDevice>>* getDeviceList();
    /// @brief Sets the current device index. Does not set the index if the index is out of range.
    void setDeviceIndex(int index);
};


#endif // BVKDEVICEMANAGER_H
