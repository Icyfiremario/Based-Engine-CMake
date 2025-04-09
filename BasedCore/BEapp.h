#pragma once

// STD
#include <string>
#include <stdexcept>
#include <memory>

// BasedCore
#include "BEwindow.h"
#include "BEenums.h"
#include "BasedConfig.h"

// BasedVK
#include "../BasedVK/BVKDevice.h"
#include "../BasedVK/BVKRenderer.h"
#include "../BasedVK/BVKRenderSystem.h"
#include "../BasedVK/BVKDescriptors.h"

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

        /// @brief Initializes app from config
        /// @param config Information loaded from config file
        /// @param name Window title
        /// @param maxFrameTime max frame time in ms
        BEapp(BasedCore::Config config, const std::string name, int maxFrameTime);

        /// @brief Destructor
        ~BEapp();

        /// @brief Begins app execution
        void run();

    private:

        int appWidth, appHeight, maxFrameTime, renderAPI;
        std::string name;

        std::unique_ptr<BEwindow> appWindow;
        std::unique_ptr<BVKDevice> appDevice;
        std::unique_ptr<BVKRenderer> appRenderer;
        
        std::unique_ptr<BVKDescriptorPool> globalPool;
};