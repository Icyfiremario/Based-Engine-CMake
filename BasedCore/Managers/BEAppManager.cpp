#include "BEAppManager.h"

#include "../../BasedVK/BVKApp.h"

BasedEngine::Managers::BEAppManager* BasedEngine::Managers::BEAppManager::instance = nullptr;
std::mutex BasedEngine::Managers::BEAppManager::mtx;

BasedEngine::Managers::BEAppManager* BasedEngine::Managers::BEAppManager::getInstance()
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

BEApp* BasedEngine::Managers::BEAppManager::createApp(const int API)
{
    switch (API)
    {
    case VULKAN:
        {
            app = std::make_unique<BVKApp>();
            PLOGI << "Created vulkan app";
            break;
        }
    case OPENGL:
    case DIRECTX:
    default:
        PLOGF << "Invalid API!";
        throw std::invalid_argument("Invalid API!");
    }

    return app.get();
}

BEApp* BasedEngine::Managers::BEAppManager::createApp(int w, int h, const std::string& title)
{
    app = std::make_unique<BVKApp>(w, h, title.c_str());
    PLOGI << "Created vulkan app";
    return app.get();
}

BEApp* BasedEngine::Managers::BEAppManager::createApp(int w, int h, std::string title, const int api)
{
    switch (api)
    {
    case VULKAN:
        {
            app = std::make_unique<BVKApp>(w, h, title.c_str());
            PLOGI << "Created vulkan app";
            break;
        }
    case OPENGL:
    case DIRECTX:
    default:
        PLOGF << "Invalid API!";
        throw std::invalid_argument("Invalid API!");
    }

    return app.get();
}
