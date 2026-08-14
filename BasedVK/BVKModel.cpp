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

void BVKModel::Builder::loadModels(const std::string& filePath)
{
}
