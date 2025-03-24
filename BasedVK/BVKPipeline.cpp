#include "BVKPipeline.h"

BVKPipeline::BVKPipeline(BVKDevice &device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo) : pipelineDevice(device)
{
    createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}