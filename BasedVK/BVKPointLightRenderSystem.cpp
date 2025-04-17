#include "BVKPointLightRenderSystem.h"

struct PointLightPushConstants {
    glm::vec4 position;
    glm::vec4 color;
    float radius;
};

BVKPointLightRenderSystem::BVKPointLightRenderSystem(BVKDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : rSysDevice{device}
{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

BVKPointLightRenderSystem::~BVKPointLightRenderSystem()
{
    vkDestroyPipelineLayout(rSysDevice.getDevice(), rSysPipelineLayout, nullptr);
}

void BVKPointLightRenderSystem::update(FrameInfo &frameInfo, GlobalUbo &ubo)
{
    auto rotateLight = glm::rotate(glm::mat4(1.f), frameInfo.frameTime, {0.f, -1.f, 0.f});

    int lightIndex = 0;
    for (auto& kv : frameInfo.appObjects)
    {
        auto& object = kv.second;

        if (object.pointLight == nullptr) continue;

        assert(lightIndex < MAX_LIGHTS && "Exceeded max number of point lights!");

        object.transform.translation = glm::vec3(rotateLight * glm::vec4(object.transform.translation, 1.f));

        ubo.pointLights[lightIndex].position = glm::vec4(object.transform.translation, 1.f);
        ubo.pointLights[lightIndex].color = glm::vec4(object.color, object.pointLight->lightIntensity);
        lightIndex++;
    }

    ubo.numLights = lightIndex;
}

void BVKPointLightRenderSystem::render(FrameInfo &frameInfo)
{
    rSysPipeline->bind(frameInfo.commandBuffer);

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rSysPipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

    for(auto& kv : frameInfo.appObjects)
    {
        auto& object = kv.second;
        if (object.pointLight == nullptr) continue;

        PointLightPushConstants push{};
        push.position = glm::vec4(object.transform.translation, 1.f);
        push.color = glm::vec4(object.color, object.pointLight->lightIntensity);
        push.radius = object.transform.scale.x;

        vkCmdPushConstants(frameInfo.commandBuffer, rSysPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstants), &push);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}

void BVKPointLightRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PointLightPushConstants);

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
