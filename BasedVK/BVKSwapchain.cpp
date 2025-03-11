#include "BVKSwapchain.h"

BVKSwapchain::BVKSwapchain(BVKDevice &deviceRef, VkExtent2D windowExtent) : device(deviceRef), windowExtent(windowExtent)
{
    init();
}

BVKSwapchain::BVKSwapchain(BVKDevice &deviceRef, VkExtent2D windowExtent, std::shared_ptr<BVKSwapchain> previous) : device(deviceRef), windowExtent(windowExtent), oldSwapchain(previous)
{
    init();
    oldSwapchain = nullptr;
}

BVKSwapchain::~BVKSwapchain()
{
}

void BVKSwapchain::createSwapChain()
{
}

void BVKSwapchain::createImageViews()
{
}

void BVKSwapchain::createDepthResources()
{
}

void BVKSwapchain::createRenderPass()
{
}

void BVKSwapchain::createFramebuffers()
{
}

void BVKSwapchain::createSyncObjects()
{
}

void BVKSwapchain::init()
{
}

VkSurfaceFormatKHR BVKSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    return VkSurfaceFormatKHR();
}

VkPresentModeKHR BVKSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    return VkPresentModeKHR();
}

VkExtent2D BVKSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    return VkExtent2D();
}
