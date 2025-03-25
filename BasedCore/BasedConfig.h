#pragma once

// STD
#include <string>

namespace BasedCore
{
    /// @brief Config srtuct to load config settings to and from files
    struct Config
    {
        int width;
        int height;
        int renderAPI;
    };
}