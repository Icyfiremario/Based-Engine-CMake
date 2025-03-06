#pragma once

// STD
#include <string>
#include <stdexcept>

// BasedCore
#include "BEwinddow.h"

// BasedGL

// BasedVK

/// @brief Core app class
class BEapp
{
    public:

        enum
        {
            OPENGL,
            VULKAN,
#ifdef WINDOWS
            DIRECTX
#endif
        };

        /// @brief Initializes necessary systems for the app
        /// @param width Window width
        /// @param height Window height
        /// @param maxFrameTime max frame time in ms
        /// @param name Window title
        /// @param api Rendering API
        BEapp(int width, int height, int maxFrameTime, std::string name, int api);
        ~BEapp();

        /// @brief Begins app execution
        void run();

    private:

        int width, height, maxFrameTime, renderAPI;
        std::string name;

        BEwindow appWindow {width, height, name};
};