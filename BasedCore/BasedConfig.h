#pragma once

// STD
#include <string>

namespace BasedCore
{
    /// @brief Config struct to load config settings to and from files
    struct Config
    {
        /// @brief Window width
        int width;
        /// @brief Window height
        int height;
        /// @brief Rendering API. (Vulkan, OpenGL, DirectX)
        int renderAPI;
    };
}