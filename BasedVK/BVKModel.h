#pragma once

// STD
#include <vector>
#include <cassert>
#include <cstring>
#include <memory>
#include <unordered_map>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

// BasedVK
#include "BVKDevice.h"
#include "BVKUtils.h"
#include "BVKBuffer.h"

class BVKModel
{
    public:

        struct Vertex
        {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
        };

        struct Builder
        {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            void loadModels(const std::string& filePath);
        };

        BVKModel(BVKDevice& device, const BVKModel::Builder& builder);
        ~BVKModel();

        BVKModel(const BVKModel&) = delete;
        BVKModel& operator=(const BVKModel&) = delete;

        static std::unique_ptr<BVKModel> createModelFromFile(BVKDevice& device, const std::string& filePath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        
        BVKDevice& modelDevice;

        std::unique_ptr<BVKBuffer> vertexBuffer;
        uint32_t vertexCount;

        std::unique_ptr<BVKBuffer> indexBuffer;
        uint32_t indexCount;
        bool hasIndexBuffer = false;

        void createVertexBuffers(const std::vector<Vertex>& vertices);
        void createIndexBuffers(const std::vector<uint32_t>& indices);

};