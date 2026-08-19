#ifndef BVKFRAMEINFO_H
#define BVKFRAMEINFO_H

// Vulkan
#include <vulkan/vulkan.h>

// BasedCore

// BasedVK
#include "BVKObject.h"

struct FrameInfo
{
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;

    // TODO: Add camera object

    VkDescriptorSet globalDescriptorSet;
    BVKObject::Map &appObjects;
};

#define MAX_LIGHTS 10

struct PointLight
{
    glm::vec4 position{}; // Ignore W
    glm::vec4 color{}; // W is intesnity
};

struct GlobalUBO
{
    glm::mat4 projection{ 1.f };
    glm::mat4 view{ 1.f };
    glm::mat4 inverseView{ 1.f };
    glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
    PointLight pointLights[MAX_LIGHTS];
    int numLights;
};

#endif // BVKFRAMEINFO_H
