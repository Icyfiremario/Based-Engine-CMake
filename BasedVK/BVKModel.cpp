#include "BVKModel.h"

// TinyObjLoader
#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyObjLoader/tiny_obj_loader.h>

namespace std
{
    template<>
    struct hash<BVKModel::Vertex>
    {
        size_t operator()(const BVKModel::Vertex& vertex) const
        {
            size_t seed = 0;
            BasedEngine::Utils::hashCombine(seed, vertex.position);
            BasedEngine::Utils::hashCombine(seed, vertex.color);
            BasedEngine::Utils::hashCombine(seed, vertex.normal);
            BasedEngine::Utils::hashCombine(seed, vertex.uv);
            return seed;
        }
    };
}

std::vector<VkVertexInputBindingDescription> BVKModel::Vertex::getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> BVKModel::Vertex::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, position)});
    attributeDescriptions.push_back({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, color)});
    attributeDescriptions.push_back({ .location = 2, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, normal)});
    attributeDescriptions.push_back({ .location = 3, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(Vertex, uv)});

    return attributeDescriptions;
}

void BVKModel::Builder::loadModels(const std::string& filePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
    {
        PLOGF << warn << err;
        throw std::runtime_error(warn + err);
    }

    vertices.clear();
    indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            if (index.vertex_index >= 0)
            {
                vertex.position = { attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2] };
                vertex.color = { attrib.colors[3 * index.vertex_index + 0], attrib.colors[3 * index.vertex_index + 1], attrib.colors[3 * index.vertex_index + 2] };
            }

            if (index.normal_index >= 0)
            {
                vertex.normal = {attrib.normals[3 * index.normal_index + 0], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2] };
            }

            if (index.texcoord_index >= 0)
            {
                vertex.uv = { attrib.texcoords[2 * index.texcoord_index + 0], attrib.texcoords[2 * index.texcoord_index + 1] };
            }

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

BVKModel::BVKModel(BVKDevice& device, const BVKModel::Builder& builder) : modelDevice(device)
{
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

BVKModel::~BVKModel()
{
}

std::unique_ptr<BVKModel> BVKModel::createModelFromFile(BVKDevice& device, const std::string& filePath)
{
    Builder builder{};
    builder.loadModels(filePath);

    return std::make_unique<BVKModel>(device, builder);
}

void BVKModel::bind(const VkCommandBuffer commandBuffer) const
{
    const VkBuffer buffers[] = { vertexBuffer->getBuffer() };
    constexpr VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    if (hasIndexBuffer)
    {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
}

void BVKModel::draw(const VkCommandBuffer commandBuffer) const
{
    if (hasIndexBuffer)
    {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    }
    else
    {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }
}

void BVKModel::createVertexBuffers(const std::vector<Vertex>& vertices)
{
    vertexCount = static_cast<uint32_t>(vertices.size());

    assert(vertexCount >= 3 && "Vertex count must be at least 3!");

    const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

    uint32_t vertexSize = sizeof(vertices[0]);

    BVKBuffer stagingBuffer{modelDevice, vertexSize, vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

    stagingBuffer.map();
    stagingBuffer.writeToBuffer(vertices.data(), bufferSize);

    vertexBuffer = std::make_unique<BVKBuffer>(modelDevice, vertexSize, vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    modelDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
}

void BVKModel::createIndexBuffers(const std::vector<uint32_t>& indices)
{
    indexCount = static_cast<uint32_t>(indices.size());

    hasIndexBuffer = indexCount > 0;

    if (!hasIndexBuffer) return;

    const VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
    uint32_t indexSize = sizeof(indices[0]);

    BVKBuffer stagingBuffer{modelDevice, indexSize, indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)indices.data());

    indexBuffer = std::make_unique<BVKBuffer>(modelDevice, indexSize, indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    modelDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
}
