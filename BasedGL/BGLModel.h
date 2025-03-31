#pragma once

// STD
#include <vector>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

// OpenGL
#include <GL/glew.h>

/// @brief BasedGL model class
class BGLModel
{
    public:

        struct Vertex
        {};

        /// @brief Binds vertex data to a buffer
        /// @param bufferType type of buffer to use. (GL_ARRAY_BUFFER)
        /// @param buffer buffer to bind to.
        /// @param data Data to bind to the buffer.
        /// @param flags Buffer flags. (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc.)
        /// @param size Size of the buffer
        void bind(uint32_t bufferType, uint32_t& buffer, const void* data, uint32_t flags, size_t size);
};