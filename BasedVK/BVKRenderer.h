#pragma once

// STD
#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

// BasedCore
#include "../BasedCore/BEwindow.h"

// BasedVK
#include "BVKDevice.h"
#include "BVKSwapchain.h"

class BVKRenderer
{
    public:

        BVKRenderer(BEwindow &window, BVKDevice &device);
        ~BVKRenderer();

        BVKRenderer(const BVKRenderer&) = delete;
        BVKRenderer& operator=(const BVKRenderer&) = delete;

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapchainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapchainRenderPass(VkCommandBuffer commandBuffer);

        bool isFrameInProgress() const { return isFrameStarted; };

        int getFrameIndex() const 
        { 
            assert(isFrameStarted && "Cannot get frame index when frame is not in progress.");
            return currentFrameIndex; 
        };

        VkRenderPass getRenderPass() const { return rendererSwapchain->getRenderPass(); };
        float getAspectRatio() const { return rendererSwapchain->extentAspectRatio(); };

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame is not in progress.");
            return commandBuffers[currentFrameIndex];
        };

    private:

        BEwindow &window;
        BVKDevice &renderDevice;
        std::unique_ptr<BVKSwapchain> rendererSwapchain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex = 0;
        bool isFrameStarted = false;

        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapchain();

};