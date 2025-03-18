#include "BVKRenderer.h"

BVKRenderer::BVKRenderer(BEwindow &window, BVKDevice &device) : window(window), renderDevice(device)
{
    recreateSwapchain();
    createCommandBuffers();
}

BVKRenderer::~BVKRenderer()
{
    freeCommandBuffers();
}

VkCommandBuffer BVKRenderer::beginFrame()
{
    assert(!isFrameStarted && "Can't being frame while one is in progress!");

    auto result = rendererSwapchain->acquireNextImage(&currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapchain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire next swapchain image!");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    return commandBuffer;
}

void BVKRenderer::endFrame()
{
    assert(isFrameStarted && "Can't end a frame if none are started!");

    auto commandBuffer = getCurrentCommandBuffer();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to end command buffer recording!");
    }

    auto result = rendererSwapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
    {
        window.resetWindowResizedFlag();
        recreateSwapchain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swapchain image!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % BVKSwapchain::MAX_FRAMES_IN_FLIGHT;
}

void BVKRenderer::beginSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted && "Can't begin a render pass if no frames are started!");
    assert(commandBuffer == getCurrentCommandBuffer() && "Cannot perform a render pass on a different frame!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = rendererSwapchain->getRenderPass();
    renderPassInfo.framebuffer = rendererSwapchain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = rendererSwapchain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(rendererSwapchain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(rendererSwapchain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{ {0, 0}, rendererSwapchain->getSwapChainExtent() };

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void BVKRenderer::endSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(isFrameStarted && "Can't end a render pass if no frames are started!");
    assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end a render pass on a different frame!");

    vkCmdEndRenderPass(commandBuffer);
}

void BVKRenderer::createCommandBuffers()
{
    commandBuffers.resize(BVKSwapchain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = renderDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(renderDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void BVKRenderer::freeCommandBuffers()
{
    vkFreeCommandBuffers(renderDevice.getDevice(), renderDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    commandBuffers.clear();
}

void BVKRenderer::recreateSwapchain()
{
    auto extent = window.getExtent();

    while (extent.width == 0 || extent.height == 0)
    {
        extent = window.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(renderDevice.getDevice());

    if (rendererSwapchain == nullptr)
    {
        rendererSwapchain = std::make_unique<BVKSwapchain>(renderDevice, extent);
    }
    else
    {
        std::shared_ptr<BVKSwapchain> oldSwapchain = std::move(rendererSwapchain);
        rendererSwapchain = std::make_unique<BVKSwapchain>(renderDevice, extent, oldSwapchain);

        if (!oldSwapchain->compareSwapFormats(*rendererSwapchain.get()))
        {
            throw std::runtime_error("Swapchain image or depth format has changed!");
        }
    }
}
