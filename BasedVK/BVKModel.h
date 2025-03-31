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

/// @brief Vulkan specific 3D model implementation
class BVKModel
{
    public:

        /// @brief Vulkan specific vertex struct
        struct Vertex
        {
            /// @brief Model position.
            glm::vec3 position{};
            /// @brief Model color.
            glm::vec3 color{};
            /// @brief Model normal matrix.
            glm::vec3 normal{};
            /// @brief Model texture map.
            glm::vec2 uv{};

            /// @brief Creates vector of information about how to bind the vertex
            /// @return std::vector of binding descriptions
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            /// @brief Creates vector of vertex attributes
            /// @return std::vector of attribute descriptions
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            /// @brief Checks this vertex against other
            /// @param other Vertex to check against
            /// @return If the vertexes match
            bool operator==(const Vertex& other) const { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
        };

        /// @brief Model builder
        struct Builder
        {
            /// @brief Model vertexes
            std::vector<Vertex> vertices;
            /// @brief Model vertex indices
            std::vector<uint32_t> indices;

            /// @brief Using TinyObjLoader load wavefront model
            /// @param filePath Model path
            void loadModels(const std::string& filePath);
        };

        /// @brief Create model from builder
        /// @param device Vulkan device
        /// @param builder Model builder
        BVKModel(BVKDevice& device, const BVKModel::Builder& builder);
        ~BVKModel();

        BVKModel(const BVKModel&) = delete;
        BVKModel& operator=(const BVKModel&) = delete;

        /// @brief Loads model from file
        /// @param device Vulkan device
        /// @param filePath Model path
        /// @return unique pointer to model
        static std::unique_ptr<BVKModel> createModelFromFile(BVKDevice& device, const std::string& filePath);

        /// @brief Binds model to command buffer
        /// @param commandBuffer Command buffer
        void bind(VkCommandBuffer commandBuffer);
        /// @brief Adds draw command to command buffer
        /// @param commandBuffer Command buffer
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