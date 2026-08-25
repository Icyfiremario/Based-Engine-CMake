#include "BEModel.h"

// TinyObjLoader
#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader/tiny_obj_loader.h"

template<>
struct std::hash<BasedEngine::Common::BEModel::Vertex>
{
    size_t operator()(const BasedEngine::Common::BEModel::Vertex& vertex) const noexcept
    {
        size_t seed = 0;
        BasedEngine::Utils::hashCombine(seed, vertex.position);
        BasedEngine::Utils::hashCombine(seed, vertex.color);
        BasedEngine::Utils::hashCombine(seed, vertex.normal);
        BasedEngine::Utils::hashCombine(seed, vertex.uv);
        return seed;
    }
};;

namespace BasedEngine::Common
{
    void BEModel::Builder::loadModels(const std::string& filePath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex, uint32_t> uniqueVertices;

        for (const auto& shape: shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex {};

                if (index.vertex_index >= 0)
                {
                    vertex.position = { attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2] };
                    vertex.color = { attrib.colors[3 * index.vertex_index + 0], attrib.colors[3 * index.vertex_index + 1], attrib.colors[3 * index.vertex_index + 2] };
                }

                if (index.normal_index >= 0)
                {
                    vertex.normal = { attrib.normals[3 * index.normal_index + 0], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]};
                }

                if (index.texcoord_index >= 0)
                {
                    vertex.uv = { attrib.texcoords[2 * index.texcoord_index + 0], attrib.texcoords[2 * index.texcoord_index + 1]};
                }

                if (!uniqueVertices.contains(vertex))
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}
