#pragma once

// STD
#include <string>
#include <stdexcept>

// BasedCore
#include "BEwinddow.h"

// BasedGL

// BasedVK

class BEapp
{
    public:

        enum
        {
            OPENGL,
            VULKAN
        };

        BEapp(int width, int height, int maxFrameTime, std::string name, int api);
        ~BEapp();

        void run();

    private:

        int width, height, maxFrameTime, renderAPI;
        std::string name;

        BEwindow appWindow {width, height, name}; 
};