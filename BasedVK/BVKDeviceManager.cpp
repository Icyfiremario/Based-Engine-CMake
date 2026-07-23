#include "BVKDeviceManager.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
#ifdef DEBUG
    std::cerr << "Validation layer: " << pCallbackData->pMessage << '\n';

    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            std::cerr << "Validation layer (info): " << pCallbackData->pMessage << '\n';
            PLOGI << "Validation layer: " << pCallbackData->pMessage;
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            std::cerr << "Validation layer (error): " << pCallbackData->pMessage << '\n';
            PLOGE << "Validation layer: " << pCallbackData->pMessage;
            break;

        default:
            std::cerr << "Validation layer: " << pCallbackData->pMessage << '\n';
            PLOGD << "Validation layer: " << pCallbackData->pMessage;
            break;
    }

#endif // DEBUG

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

BVKDeviceManager::BVKDeviceManager()
{
    // Log BVKDeviceManager init
    PLOGI << "Vulkan device manager initialized.";

    // Create vulkan instance, pick device, create VkDevice Object
    createInstance();
    setupDebugMessenger();
    createSurface();
    findDevices();
}

BVKDeviceManager::BVKDeviceManager(BVKWindow* window) : m_window(window)
{
    PLOGI << "Vulkan device managerinitialized.";

    createInstance();
    setupDebugMessenger();
    createSurface();
    findDevices();
}

void BVKDeviceManager::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        //throw std::runtime_error("Validation Layers are not supported!");
        std::cerr << "WARNING: Validation layer support was not found.\n";
        PLOGW << "Validation layer support was not found.";
        enableValidationLayers = false;
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "BasedEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "No engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.flags |= FLAGS;

    const auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (const VkResult result = vkCreateInstance(&createInfo, nullptr, &instance); result != VK_SUCCESS)
    {
        std::string reason;

        switch (result)
        {
        case -9:
            reason = "incompatible driver";
            break;
        default:
            reason = "unknown";
            break;
        }

        PLOGF << "Failed to create Vulkan instance: " << reason;
        throw std::runtime_error("Failed to create Vulkan instance: " + reason);
    }

    PLOGI << "Vulkan instance created.";
}

void BVKDeviceManager::setupDebugMessenger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create debug messenger!");
    }

    PLOGI << "Created debug messenger.";
}

void BVKDeviceManager::createSurface(){ m_window->createWindowSurface(instance, &surface_); }

void BVKDeviceManager::findDevices()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("Failed to find Vulkan compatible GPUs! Check your GPU manufacture's website to get the latest drivers.");
    }

    PLOGI << "Found " << deviceCount << " compatible GPU(s).";

    if (logDevice)
    {
        std::cout << "Device count: " << deviceCount << '\n';
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    devices.reserve(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    for (const auto& physicalDevice : physicalDevices)
    {
        auto currentDevice = std::make_shared<BVKDevice>(physicalDevice, m_window, &surface_);

    }
}

bool BVKDeviceManager::isDeviceSuitable(VkPhysicalDevice device)
{
}

std::vector<const char*> BVKDeviceManager::getRequiredExtensions() const
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if constexpr (FLAGS != 0)
    {
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    }

    return extensions;
}

bool BVKDeviceManager::checkValidationLayerSupport() const
{
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

void BVKDeviceManager::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}

BVKDeviceManager::BVKDeviceManager(Token) : BVKDeviceManager() {}

BVKDeviceManager::BVKDeviceManager(Token, BVKWindow* window) : BVKDeviceManager(window) {}

BVKDeviceManager::~BVKDeviceManager()
{
    for (auto & device : devices)
    {
        device.reset();
    }

    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroyInstance(instance, nullptr);

    PLOGI << "BVK device manager released.";
}

std::shared_ptr<BVKDeviceManager> BVKDeviceManager::getInstance()
{
    static auto instance = std::make_shared<BVKDeviceManager>(Token{});
    return instance;
}

std::shared_ptr<BVKDeviceManager> BVKDeviceManager::getInstance(BVKWindow* window)
{
    static auto instance = std::make_shared<BVKDeviceManager>(Token{}, window);
    return instance;
}

std::shared_ptr<BVKDevice> BVKDeviceManager::getDevicePtr()
{
    if (static_cast<size_t>(currentDeviceIndex) >= devices.size() || !devices[currentDeviceIndex])
    {
        PLOGE << "Failed to get device pointer!";
        return nullptr;
    }

    return devices[currentDeviceIndex];
}

std::vector<std::shared_ptr<BVKDevice>>* BVKDeviceManager::getDeviceList()
{
    return &devices;
}

void BVKDeviceManager::setDeviceIndex(const int index)
{
    if (index < 0 || static_cast<size_t>(index) >= devices.size())
    {
        PLOGE << "Index " << index << " is outside of the device list bounds. Keeping current value.";
        return;
    }

    currentDeviceIndex = index;
}
