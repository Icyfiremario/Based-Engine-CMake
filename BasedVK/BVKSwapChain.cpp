#include "BVKSwapChain.h"

BVKSwapChain::BVKSwapChain(BVKDevice& deviceRef, const VkExtent2D windowExtent) : device(deviceRef), windowExtent(windowExtent)
{
    init();
}

BVKSwapChain::BVKSwapChain(BVKDevice& deviceRef, const VkExtent2D windowExtent, const std::shared_ptr<BVKSwapChain>& previous) : device(deviceRef), windowExtent(windowExtent), oldSwapChain(previous)
{
    init();
    oldSwapChain = nullptr;
}

BVKSwapChain::~BVKSwapChain()
{
    for (const auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(device.getDevice(), imageView, nullptr);
    }
    swapChainImageViews.clear();

    if (swapChain != nullptr)
    {
        vkDestroySwapchainKHR(device.getDevice(), swapChain, nullptr);
        swapChain = nullptr;
    }

    for (size_t i = 0; i < depthImages.size(); i++)
    {
        vkDestroyImageView(device.getDevice(), depthImageViews[i], nullptr);
        vkDestroyImage(device.getDevice(), depthImages[i], nullptr);
        vkFreeMemory(device.getDevice(), depthImageMemories[i], nullptr);
    }

    for (const auto frameBuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(device.getDevice(), frameBuffer, nullptr);
    }
    vkDestroyRenderPass(device.getDevice(), renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(device.getDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device.getDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device.getDevice(), inFlightFences[i], nullptr);
    }

}

void BVKSwapChain::createSwapChain()
{
    const SwapChainSupportDetails swapChainSupport = device.getSwapChainSupport();

    const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    const VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    const VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = device.getSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = device.findPhysicalQueueFamilies();
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = oldSwapChain == nullptr ? VK_NULL_HANDLE : oldSwapChain->swapChain;

    if (vkCreateSwapchainKHR(device.getDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        PLOGF << "Failed to create swap chain.";
        throw std::runtime_error("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device.getDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device.getDevice(), swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    PLOGI << "Created swapchain.";
}

void BVKSwapChain::createImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo viewInfo{};
    }
}

void BVKSwapChain::init()
{
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDepthResources();
    createFramebuffers();
    createSyncObjects();
}
