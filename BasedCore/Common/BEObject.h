#ifndef BEOBJECT_H
#define BEOBJECT_H

// STD
#include <memory>
#include <unordered_map>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// BasedCore
#include "BEModel.h"

namespace BasedEngine::Common
{
    // TODO: Create ECS system for the BEObject class

    struct TransformComponent
    {
        glm::vec3 translation {};
        glm::vec3 scale {1.f, 1.f, 1.f};
        glm::vec3 rotation {};

        glm::mat4 mat4();
        glm::mat4 normalMatrix() const;
    };

    struct PointLightComponent // More for the vulkan point light shader than for general game objects.
    {
        float lightIntensity { 1.f };
    };

    class BEObject
    {
    public:

        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, BEObject>;

        std::shared_ptr<BEModel> model;
        glm::vec3 color {};
        TransformComponent transform {};

        std::unique_ptr<PointLightComponent> pointLight = nullptr;

        static BEObject createGameObject()
        {
            static id_t currentId = 0;
            return BEObject{currentId++ };
        }

        BEObject(const BEObject&) = delete;
        BEObject& operator=(const BEObject&) = delete;
        BEObject(BEObject&&) = default;
        BEObject& operator=(BEObject&&) = default;

        static BEObject makePointLight(float intensity = 10.f, float radius = .1f, glm::vec3 color = glm::vec3(1.f));

        id_t getId() const { return id; };

    private:
        id_t id;

        explicit BEObject(const id_t id) : id(id) {}
    };
}

#endif // BEOBJECT_H
