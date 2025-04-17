#pragma once

// STD
#include <memory>
#include <vector>
#include <cassert>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// BasedVK
#include "BVKDevice.h"
#include "BVKObject.h"
#include "BVKPipeline.h"
#include "BVKFrameInfo.h"

// BasedCore
#include "../BasedCore/BECamera.h"

/// @brief Render system
class BVKRenderSystem
{
    public:

        /// @brief Creates render system using the Vulkan device, a render pass, and a global descriptor set.
        /// @param device Vulkan device
        /// @param renderPass render pass
        /// @param globalSetLayout descriptor set layout
        BVKRenderSystem(BVKDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~BVKRenderSystem();

        BVKRenderSystem(const BVKRenderSystem&) = delete;
        BVKRenderSystem& operator=(const BVKRenderSystem&) = delete;

        /// @brief Renders game objects according to frame info and camera position
        /// @param frameInfo Frame info
        /// @param gameObjects List of game objects to render
        void renderGameObjects(FrameInfo& frameInfo);

    private:

        BVKDevice& rSysDevice;
    
        std::unique_ptr<BVKPipeline> rSysPipeline;
        VkPipelineLayout rSysPipelineLayout;
    
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
};