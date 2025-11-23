#include "BVKModel.h"

std::vector<VkVertexInputBindingDescription> BVKModel::VKVertex::getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(VKVertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> BVKModel::VKVertex::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VKVertex, position)});
    attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VKVertex, color)});
    attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VKVertex, normal)});
    attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VKVertex, uv)});
}

BVKModel::BVKModel(BVKDevice& device, const Builder& builder) : m_device(device)
{
    createVertexBuffer(builder.vertices);
    createIndexBuffer(builder.indices);
}

std::unique_ptr<BVKModel> BVKModel::createModelFromFile(BVKDevice& device, const std::string& filePath)
{
    Builder builder {};
    builder.loadModels(filePath);

    return std::make_unique<BVKModel>(device, builder);
}

void BVKModel::bind(VkCommandBuffer commandBuffer)
{
    throw std::logic_error("Not implemented yet!");
}

void BVKModel::draw(VkCommandBuffer commandBuffer)
{
    throw std::logic_error("Not implemented yet!");
}

void BVKModel::createVertexBuffer(const std::vector<Vertex>& vertices)
{
}

void BVKModel::createIndexBuffer(const std::vector<uint32_t>& indices)
{
}
