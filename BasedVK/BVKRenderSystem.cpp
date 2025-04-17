#include "BVKRenderSystem.h"

struct SimplePushConstantData
{
    glm::mat4 modelMatrix{ 1.f };
    glm::mat4 normalMatrix{ 1.f };
};

BVKRenderSystem::BVKRenderSystem(BVKDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : rSysDevice{device}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

BVKRenderSystem::~BVKRenderSystem()
{
    vkDestroyPipelineLayout(rSysDevice.getDevice(), rSysPipelineLayout, nullptr);
}

void BVKRenderSystem::renderGameObjects(FrameInfo &frameInfo)
{
    rSysPipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rSysPipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for (auto& kv : frameInfo.appObjects)
    {
        auto& obj = kv.second;
        if (obj.model == nullptr) continue;
        SimplePushConstantData push{};
        push.modelMatrix = obj.transform.mat4();
        push.normalMatrix = obj.transform.normalMatrix();

        vkCmdPushConstants(frameInfo.commandBuffer, rSysPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
        obj.model->bind(frameInfo.commandBuffer);
        obj.model->draw(frameInfo.commandBuffer);
    }
}

void BVKRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(rSysDevice.getDevice(), &pipelineLayoutInfo, nullptr, &rSysPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void BVKRenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(rSysPipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

    PipelineConfigInfo pipelineConfig {};

    BVKPipeline::defaultPipelineConfigInfo(pipelineConfig);

    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = rSysPipelineLayout;

    rSysPipeline = std::make_unique<BVKPipeline>(rSysDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
}
