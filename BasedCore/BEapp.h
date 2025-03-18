#pragma once

// STD
#include <string>
#include <stdexcept>
#include <memory>

// BasedCore
#include "BEwindow.h"
#include "BEenums.h"

// BasedVK
#include "../BasedVK/BVKDevice.h"
#include "../BasedVK/BVKRenderer.h"

// BasedGL
#include "../BasedGL/BGLShader.h"

/// @brief Core app class
class BEapp
{
    public:

        /// @brief Initializes necessary systems for the app
        /// @param width Window width
        /// @param height Window height
        /// @param maxFrameTime max frame time in ms
        /// @param name Window title
        /// @param api Rendering API
        BEapp(int width, int height, int maxFrameTime, const std::string name, int api);
        ~BEapp();

        /// @brief Begins app execution
        void run();

    private:

        int appWidth, appHeight, maxFrameTime, renderAPI;
        std::string name;

        std::unique_ptr<BEwindow> appWindow;
        std::unique_ptr<BVKDevice> appDevice;
        std::unique_ptr<BVKRenderer> appRenderer;
        
        //BEwindow appWindow {800, 600, "BasedEngine", renderAPI};
        //BVKDevice appDevice {appWindow};
        //BVKRenderer appRenderer {appWindow, appDevice};

};