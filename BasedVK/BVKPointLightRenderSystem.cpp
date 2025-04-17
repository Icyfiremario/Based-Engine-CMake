#include "BVKPointLightRenderSystem.h"

BVKPointLightRenderSystem::BVKPointLightRenderSystem(BVKDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : rSysDevice{device}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

BVKPointLightRenderSystem::~BVKPointLightRenderSystem()
{
    vkDestroyPipelineLayout(rSysDevice.getDevice(), rSysPipelineLayout, nullptr);
}

void BVKPointLightRenderSystem::render(FrameInfo &frameInfo)
{
    rSysPipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rSysPipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
}

void BVKPointLightRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    //VkPushConstantRange pushConstantRange{};
    //pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    //pushConstantRange.offset = 0;
    //pushConstantRange.size = sizeof(SimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(rSysDevice.getDevice(), &pipelineLayoutInfo, nullptr, &rSysPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void BVKPointLightRenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(rSysPipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

    PipelineConfigInfo pipelineConfig {};

    BVKPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = rSysPipelineLayout;

    rSysPipeline = std::make_unique<BVKPipeline>(rSysDevice, "shaders/point_light.vert.spv", "shaders/point_light.frag.spv", pipelineConfig);
}
