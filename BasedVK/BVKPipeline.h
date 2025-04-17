#pragma once

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
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

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
    std::string path;
    VkStructureType shaderType;
};

class BVKPipeline
{
    public:

        /// @brief Loads a vertex shader and a fragment shader in that order and creates a graphics pipeline.
        /// @param device Vulkan device
        /// @param vertFilepath Vertex shader file path
        /// @param fragFilepath Fragment shader file path
        /// @param configInfo Pipeline config
        BVKPipeline(BVKDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
        /// @brief Loads a list of shaders and creates a pipeline that uses them in that order.
        /// @param device Vulkan device
        /// @param shaderFilePaths Shader list
        /// @param configInfo Pipeline config
        BVKPipeline(BVKDevice& device, const std::vector<std::string> shaderFilePaths, const PipelineConfigInfo& configInfo);
        ~BVKPipeline();

        BVKPipeline(const BVKPipeline&) = delete;
        BVKPipeline& operator=(const BVKPipeline&) = delete;

        /// @brief Binds a pipeline to a frame.
        /// @param commandBuffer Frame command buffer.
        void bind(VkCommandBuffer commandBuffer);

        /// @brief Sets default pipeline config info.
        /// @param configInfo pipeline info object.
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:

        BVKDevice& pipelineDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

        std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
        void createGraphicsPipeline(const std::vector<std::string> shaderFilePaths, const PipelineConfigInfo& configInfo);
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
};

