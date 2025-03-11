#pragma once

namespace BasedCore
{
    /// @brief Rendering APIs
    enum RenderAPIs
    {
        OPENGL,
        VULKAN,
#ifdef WINDOWS
            DIRECTX
#endif
    };
}