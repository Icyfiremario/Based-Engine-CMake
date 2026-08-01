#ifndef BASEDENGINE_BVKSWAPCHAIN_H
#define BASEDENGINE_BVKSWAPCHAIN_H

// STD
#include <memory>
#include <vector>
#include <array>
#include <set>

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

private:
#ifdef DEBUG
    const bool logSwapchain = true;
#else
    const bool logSwapchain = false;
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
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
};


#endif //BASEDENGINE_BVKSWAPCHAIN_H
