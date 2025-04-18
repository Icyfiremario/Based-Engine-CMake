#pragma once

// Vulkan
#include <vulkan/vulkan.h>

// BasedCore
#include "../BasedCore/BECamera.h"

// BasedVK
#include "BVKObject.h"

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
    BVKObject::Map &appObjects;
};

#define MAX_LIGHTS 10

struct PointLight
{
    glm::vec4 position{}; // ignore w
    glm::vec4 color{}; // W is intensity
};

struct GlobalUbo
{
    /// @brief projection view
    glm::mat4 projection{ 1.f };
    glm::mat4 view{ 1.f };
    glm::mat4 inverseView{ 1.f};
    glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f }; // w is intensity
    PointLight pointLights[MAX_LIGHTS];
    int numLights;
};