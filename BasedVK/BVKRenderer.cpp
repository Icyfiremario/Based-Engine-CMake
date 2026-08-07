#include "BVKRenderer.h"

BVKRenderer::BVKRenderer(BVKWindow& window, BVKDevice& device) : window(window), renderDevice(device)
{
    recreateSwapChain();
    createCommandBuffers();
}

BVKRenderer::~BVKRenderer()
{
    freeCommandBuffers();
}

VkCommandBuffer BVKRenderer::beginFrame()
{
    assert(!isFrameStarted && "Can't begin frame while one is in progress!");

    if (const auto result = rendererSwapChain->acquireNextImage(&currentImageIndex); result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return nullptr;
    }

    isFrameStarted = true;

    const auto commandBuffer = getCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        PLOGF << "Failed to begin recording command buffer.";
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    PLOGI << "Started frame.";

    return commandBuffer;
}

void BVKRenderer::endFrame()
{
    assert(isFrameStarted && "Can't end frame if none are started!");

    const auto commandBuffer = getCurrentCommandBuffer();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        PLOGF << "Failed to end command buffer recording.";
        throw std::runtime_error("Failed to end command buffer recording!");
    }

    if (const auto result = rendererSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex); result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
    {
        window.resetWindowResizedFlag();
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        PLOGF << "Failed to present swap chain image.";
        throw std::runtime_error("Failed to present swap chain image!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % BVKSwapChain::MAX_FRAMES_IN_FLIGHT;

    PLOGI << "Ended frame.";
}

void BVKRenderer::beginSwapChainRenderPass(const VkCommandBuffer commandBuffer) const
{
    assert(isFrameStarted && "Can't begin a renderpass if no frames are started!");
    assert(commandBuffer == getCurrentCommandBuffer() && "Cannot perform a render pass on a different frame!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = rendererSwapChain->getRenderPass();
    renderPassInfo.framebuffer = rendererSwapChain->getFramebuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = { .x = 0, .y = 0 };
    renderPassInfo.renderArea.extent = rendererSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {.depth = 1.0f, .stencil = 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(rendererSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(rendererSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    const VkRect2D scissor{.offset = {.x = 0, .y = 0}, .extent = rendererSwapChain->getSwapChainExtent() };

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void BVKRenderer::endSwapChainRenderPass(const VkCommandBuffer commandBuffer) const
{
    assert(isFrameStarted && "Can't end a render pass if no frames are started!");
    assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end a render pass on a different frame!");

    vkCmdEndRenderPass(commandBuffer);
}

void BVKRenderer::createCommandBuffers()
{
    commandBuffers.resize(BVKSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = renderDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(renderDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        PLOGF << "Failed to allocate command buffers.";
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    PLOGI << "Command buffers allocated.";
}

void BVKRenderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(renderDevice.getDevice(), renderDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    commandBuffers.clear();

    PLOGI << "Freed command buffers.";
}

void BVKRenderer::recreateSwapChain()
{
    auto extent = window.getExtent();

    while (extent.width == 0 || extent.height == 0)
    {
        extent = window.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(renderDevice.getDevice());

    if (rendererSwapChain == nullptr)
    {
        rendererSwapChain = std::make_unique<BVKSwapChain>(renderDevice, extent);
    }
    else
    {
        std::shared_ptr<BVKSwapChain> oldSwapChain = std::move(rendererSwapChain);
        rendererSwapChain = std::make_unique<BVKSwapChain>(renderDevice, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*rendererSwapChain))
        {
            PLOGF << "Swap chain image or depth format has changed.";
            throw std::runtime_error("Failed to recreate swap chain image!");
        }
    }

    PLOGI << "Recreated swap chain.";
}
