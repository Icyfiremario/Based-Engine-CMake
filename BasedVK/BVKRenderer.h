#ifndef BVKRENDERER_H
#define BVKRENDERER_H

// STD
#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

// BasedVK
#include "BVKWindow.h"
#include "BVKDevice.h"
#include "BVKSwapChain.h"

class BVKRenderer
{
public:

    BVKRenderer(BVKWindow &window, BVKDevice &device);
    ~BVKRenderer();

    BVKRenderer(const BVKRenderer&) = delete;
    BVKRenderer& operator=(const BVKRenderer&) = delete;

    VkCommandBuffer beginFrame();
    void endFrame();

    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer) const;
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer) const;

    [[nodiscard]] bool isFrameInProgress() const { return isFrameStarted; }

    [[nodiscard]] int getFrameIndex() const
    {
        assert(isFrameStarted && "Cannot get frame index when frame is not in progress.");
        return currentFrameIndex;
    }

    [[nodiscard]] VkRenderPass getRenderPass() const { return rendererSwapChain->getRenderPass(); }
    [[nodiscard]] float getAspectRatio() const { return rendererSwapChain->extentAspectRatio(); }

    [[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(isFrameStarted && "Cannot get command buffer when frame is not in progress.");
        return commandBuffers[currentFrameIndex];
    }

private:

    BVKWindow &window;
    BVKDevice &renderDevice;
    std::unique_ptr<BVKSwapChain> rendererSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex = 0;
    int currentFrameIndex = 0;
    bool isFrameStarted = false;

    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();
};


#endif // BVKRENDERER_H
