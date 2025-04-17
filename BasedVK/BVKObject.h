#pragma once

// STD
#include <memory>
#include <unordered_map>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// BasedVK
#include "BVKModel.h"

struct TransformComponent
{
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    glm::mat4 mat4();
    glm::mat3 normalMatrix();
};

struct PointLightComponent
{
    float lightIntensity{ 1.f };
};

class BVKObject
{
    public:

        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, BVKObject>;

        std::shared_ptr<BVKModel> model;
        glm::vec3 color{};
        TransformComponent transform{};

        std::unique_ptr<PointLightComponent> pointLight = nullptr;

        static BVKObject createGameObject()
        {
            static id_t currentId = 0;

            return BVKObject{ currentId++ };
        }

        BVKObject(const BVKObject&) = delete;
        BVKObject& operator=(const BVKObject&) = delete;
        BVKObject(BVKObject&&) = default;
        BVKObject& operator=(BVKObject&&) = default;

        static BVKObject makePointLight(float intensity = 10.f, float radius = .1f, glm::vec3 color = glm::vec3(1.f));

        id_t getId() { return id; }

    private:
        
        id_t id;

        BVKObject(id_t objectId) : id{ objectId } {}
};