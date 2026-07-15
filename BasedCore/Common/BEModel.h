#ifndef BEMODEL_H
#define BEMODEL_H

// STD
#include <vector>
#include <cassert>
#include <cstring>
#include <unordered_map>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

// BasedCore
#include "../BEUtils.h"

namespace BasedEngine::Common
{
    class BEModel
    {
    public:
        virtual ~BEModel() = default;

        struct Vertex
        {
            glm::vec3 position {};
            glm::vec3 color {};
            glm::vec3 normal {};
            glm::vec2 uv {};

            bool operator==(const Vertex& other) const { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
        };

        struct Builder
        {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            void loadModels(const std::string& filePath);
        };

    private:

        virtual void createVertexBuffer(const std::vector<Vertex>& vertices);
        virtual void createIndexBuffer(const std::vector<uint32_t>& indices);
    };

}

#endif // BEMODEL_H
