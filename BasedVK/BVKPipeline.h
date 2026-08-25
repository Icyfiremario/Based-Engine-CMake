#ifndef BVKPIPELINE_H
#define BVKPIPELINE_H

// STD
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

// BasedVK
#include "BVKDevice.h"
#include "BVKModel.h"

struct PipelineConfigInfo
{
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo &) = delete;

    std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;

    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;

    uint32_t subpass = 0;
};

struct ShaderInfo
{
    std::string name;
    VkStructureType shaderType;
};

class BVKPipeline
{
public:

    BVKPipeline(BVKDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

    BVKPipeline(BVKDevice& device, const std::vector<std::string>& shaderFilePaths, const PipelineConfigInfo& configInfo);
    ~BVKPipeline();

    BVKPipeline(const BVKPipeline &) = delete;
    BVKPipeline &operator=(const BVKPipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer) const;

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    static void enableAlphaBlending(PipelineConfigInfo& configInfo);

private:

    BVKDevice& pipelineDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    std::vector<char> readFile(const std::string& filePath);

    void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
    void createGraphicsPipeline(std::vector<std::string> shaderFilePaths, const PipelineConfigInfo& configInfo);
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const;

};

#endif // BVKPIPELINE_H
