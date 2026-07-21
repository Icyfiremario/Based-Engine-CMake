#include "BEAppManager.h"

#include "../../BasedVK/BVKApp.h"

BEAppManager* BEAppManager::instance = nullptr;
std::mutex BEAppManager::mtx;

BEAppManager* BEAppManager::getInstance()
{
    if (instance == nullptr)
    {
        std::lock_guard lock(mtx);
        if (instance == nullptr)
        {
            instance = new BEAppManager();
        }
    }

    return instance;
}

BEApp* BEAppManager::createApp(const int API)
{
    switch (API)
    {
    case BasedEngine::VULKAN:
        {
            app = std::make_unique<BVKApp>();
            PLOGI << "Created vulkan app";
            break;
        }
    case BasedEngine::OPENGL:
    case BasedEngine::DIRECTX:
    default:
        PLOGF << "Invalid API!";
        throw std::invalid_argument("Invalid API!");
    }

    return app.get();
}

BEApp* BEAppManager::createApp(int w, int h, const std::string& title)
{
    app = std::make_unique<BVKApp>(w, h, title.c_str());
    PLOGI << "Created vulkan app";
    return app.get();
}

BEApp* BEAppManager::createApp(int w, int h, std::string title, const int api)
{
    switch (api)
    {
    case BasedEngine::VULKAN:
        {
            app = std::make_unique<BVKApp>(w, h, title.c_str());
            PLOGI << "Created vulkan app";
            break;
        }
    case BasedEngine::OPENGL:
    case BasedEngine::DIRECTX:
    default:
        PLOGF << "Invalid API!";
        throw std::invalid_argument("Invalid API!");
    }

    return app.get();
}
