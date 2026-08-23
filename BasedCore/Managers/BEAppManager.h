#ifndef BEAPPMANAGER_H
#define BEAPPMANAGER_H

// STD
#include <mutex>
#include <string>
#include <stdexcept>
#include <memory>

// BasedCore
#include "../BEUtils.h"
#include "../Common/BEApp.h"

// BasedVK
#include "../../BasedVK/BVKApp.h"

// BasedGL
#include "../../BasedGL/BGLApp.h"

#ifdef DIRECTX_AVAILABLE
// BasedDX
#include "../../BasedDX/BDXApp.h"
#endif


namespace BasedEngine::Managers
{
    class BEAppManager
    {

    public:

        BEAppManager(const BEAppManager&) = delete;

        static BEAppManager* getInstance();

        /// @brief Creates a Based Engine app with the specified API using the default window width, height, and title.
        BEApp* createApp(API api);
        /// @brief Creates a Vulkan Based Engine app with the specified width, height, and title.
        BEApp* createApp(int w, int h, const std::string& title);
        ///// @brief Creates a Based Engine app using the specified values.
        BEApp* createApp(int w, int h, const std::string& title, API api);

        [[nodiscard]] BEApp* getApp() const { return app.get(); }

    private:

        static BEAppManager* instance;
        static std::mutex mtx;

        std::unique_ptr<BEApp> app;

        BEAppManager() = default;
        ~BEAppManager() = default;
    };
}

#endif // BEAPPMANAGER_H