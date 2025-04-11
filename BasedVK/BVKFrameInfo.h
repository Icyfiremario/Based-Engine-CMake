#pragma once

// Vulkan
#include <vulkan/vulkan.h>

// BasedCore
#include "../BasedCore/BECamera.h"

/// @brief Frame info to tell the render system how to render
struct FrameInfo
{
    /// @brief Frame index
    int frameIndex;
    /// @brief Frame time
    float frameTime;
    /// @brief Current command buffer
    VkCommandBuffer commandBuffer;
    /// @brief Camera
    BECamera& camera;
    /// @brief Descriptors
    VkDescriptorSet globalDescriptorSet;
};