#ifndef BVKRENDERSYSTEM_H
#define BVKRENDERSYSTEM_H

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
#include "BVKPipeline.h"
#include "BVKFrameInfo.h"

class BVKRenderSystem
{
public:

    BVKRenderSystem(BVKDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~BVKRenderSystem();

    BVKRenderSystem(const BVKRenderSystem &) = delete;
    BVKRenderSystem &operator=(const BVKRenderSystem &) = delete;

    void renderGameObjects(const FrameInfo& frameInfo) const;

private:

    BVKDevice &rSysDevice;

    std::unique_ptr<BVKPipeline> rSysPipeline;
    VkPipelineLayout rSysPipelineLayout;

    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);
};


#endif // BVKRENDERSYSTEM_H
