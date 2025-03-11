#pragma once

// STD
#include <vector>
#include <set>
#include <string>
#include <array>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <memory>
#include <limits>

// BasedVK
#include "BVKDevice.h"

// Vulkan
#include <vulkan/vulkan.h>

class BVKSwapchain
{
    public:

        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        BVKSwapchain(BVKDevice &deviceRef, VkExtent2D windowExtent);
        BVKSwapchain(BVKDevice &deviceRef, VkExtent2D windowExtent, std::shared_ptr<BVKSwapchain> previous);
        ~BVKSwapchain();

        BVKSwapchain(const BVKSwapchain&) = delete;
        BVKSwapchain& operator=(const BVKSwapchain&) = delete;
        
        VkFramebuffer getFrameBuffer(int index) const { return swapChainFramebuffers[index]; };
        VkRenderPass getRenderPass() const { return renderPass; };
        VkImageView getImageView(int index) const { return swapChainImageViews[index]; };
        size_t imageCount() const { return swapChainImages.size(); };
        VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; };
        VkExtent2D getSwapChainExtent() const { return swapChainExtent; };
        uint32_t width() const { return swapChainExtent.width; };
        uint32_t height() const { return swapChainExtent.height; };

        float extentAspectRatio() const { return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height); };

    private:
    
#ifdef NDEBUG
        const bool logSwapChain = false;
#else
        const bool logSwapChain = true;
#endif 

        BVKDevice &device;

        VkFormat swapChainImageFormat;
        VkFormat depthImageFormat;
        VkExtent2D swapChainExtent;
        VkExtent2D windowExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemory;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        VkSwapchainKHR swapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;

        std::shared_ptr<BVKSwapchain> oldSwapchain;

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