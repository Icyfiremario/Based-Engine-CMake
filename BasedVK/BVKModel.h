#ifndef BVKMODEL_H
#define BVKMODEL_H


// BasedCore
#include "../BasedCore/Common/BEModel.h"

// BasedVK
#include "BVKDevice.h"
#include "BVKBuffer.h"

class BVKModel final : BasedEngine::Common::BEModel
{
public:

    struct VKVertex : BasedEngine::Common::BEModel::Vertex
    {
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    BVKModel(BVKDevice& device, const Builder& builder);

    static std::unique_ptr<BVKModel> createModelFromFile(BVKDevice& device, const std::string& filePath);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:

    BVKDevice& m_device;

    std::unique_ptr<BVKBuffer> vertexBuffer;
    uint32_t vertexCount;

    std::unique_ptr<BVKBuffer> indexBuffer;
    uint32_t indexCount;

    bool hasIndexBuffer = false;

    void createVertexBuffer(const std::vector<Vertex>& vertices) override;
    void createIndexBuffer(const std::vector<uint32_t>& indices) override;
};


#endif // BVKMODEL_H