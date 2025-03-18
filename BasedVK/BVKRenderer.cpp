#include "BVKRenderer.h"

BVKRenderer::BVKRenderer(BEwindow &window, BVKDevice &device) : window(window), renderDevice(device)
{
    createCommandBuffers();
}

BVKRenderer::~BVKRenderer()
{
}

VkCommandBuffer BVKRenderer::beginFrame()
{
    return VkCommandBuffer();
}

void BVKRenderer::endFrame()
{
}

void BVKRenderer::beginSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
}

void BVKRenderer::endSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
}

void BVKRenderer::createCommandBuffers()
{
}

void BVKRenderer::freeCommandBuffers()
{
}

void BVKRenderer::recreateSwapchain()
{
}
