#pragma once

// Vulkan
#include <vulkan/vulkan.h>

// BasedCore
#include "../BasedCore/BECamera.h"

struct FrameInfo
{
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    BECamera& camera;
    VkDescriptorSet globalDescriptorSet;
};