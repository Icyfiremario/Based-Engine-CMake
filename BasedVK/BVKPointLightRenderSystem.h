#pragma once

// STD
#include <memory>
#include <vector>

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

/// @brief Point light render system
class BVKPointLightRenderSystem
{
    public:
        BVKPointLightRenderSystem(BVKDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~BVKPointLightRenderSystem();

        BVKPointLightRenderSystem(const BVKPointLightRenderSystem&) = delete;
        BVKPointLightRenderSystem& operator=(const BVKPointLightRenderSystem&) = delete;

        void update(FrameInfo& frameInfo, GlobalUbo& ubo);
        void render(FrameInfo& frameInfo);

    private:
        BVKDevice& rSysDevice;

        std::unique_ptr<BVKPipeline> rSysPipeline;
        VkPipelineLayout rSysPipelineLayout;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
};