#ifndef BVKSWAPCHAIN_H
#define BVKSWAPCHAIN_H

// STD
#include <memory>
#include <vector>
#include <array>
#include <set>
#include <limits>
#include <iostream>

// Vulkan
#include <vulkan/vulkan.h>

// BasedVK
#include "BVKDevice.h"

class BVKSwapChain
{
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    BVKSwapChain(BVKDevice &deviceRef, VkExtent2D windowExtent);
    BVKSwapChain(BVKDevice &deviceRef, VkExtent2D windowExtent, const std::shared_ptr<BVKSwapChain>& previous);
    ~BVKSwapChain();

    BVKSwapChain(const BVKSwapChain &) = delete;
    BVKSwapChain &operator=(const BVKSwapChain &) = delete;

    [[nodiscard]] VkFramebuffer getFramebuffer(const int index) const { return swapChainFramebuffers[index]; }
    [[nodiscard]] VkRenderPass getRenderPass() const { return renderPass; }
    [[nodiscard]] VkImageView getImageView(const int index) const { return swapChainImageViews[index]; }
    [[nodiscard]] size_t imageCount() const { return swapChainImages.size(); }
    [[nodiscard]] VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }
    [[nodiscard]] VkExtent2D getSwapChainExtent() const { return swapChainExtent; }
    [[nodiscard]] uint32_t width() const { return swapChainExtent.width; }
    [[nodiscard]] uint32_t height() const { return swapChainExtent.height; }

    [[nodiscard]] float extentAspectRatio() const { return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height); }

    [[nodiscard]] VkFormat findDepthFormat() const;

    VkResult acquireNextImage(uint32_t* imageIndex) const;
    VkResult submitCommandBuffers(const VkCommandBuffer* buffers, const uint32_t* imageIndex);

    [[nodiscard]] bool compareSwapFormats(const BVKSwapChain& swapChain) const { return swapChain.swapChainDepthFormat == swapChainDepthFormat && swapChain.swapChainImageFormat == swapChainImageFormat; }

private:
#ifdef DEBUG
    const bool logSwapChain = true;
#else
    const bool logSwapChain = false;
#endif

    BVKDevice &device;

    VkFormat swapChainImageFormat;
    VkFormat swapChainDepthFormat;
    VkExtent2D swapChainExtent;
    VkExtent2D windowExtent;

    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkRenderPass renderPass;

    std::vector<VkImage> depthImages;
    std::vector<VkDeviceMemory> depthImageMemories;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    VkSwapchainKHR swapChain;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    std::shared_ptr<BVKSwapChain> oldSwapChain;

    size_t currentFrame = 0;

    void createSwapChain();
    void createImageViews();
    void createDepthResources();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();

    void init();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const;
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;
};


#endif // BVKSWAPCHAIN_H
