#include "BEAppManager.h"

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

BEApp* BasedEngine::Managers::BEAppManager::createApp(const API api)
{
    switch (api)
    {
    case VULKAN:
        {
            app = std::make_unique<BVKApp>();
            PLOGI << "Created Vulkan app.";
            break;
        }
    case OPENGL:
        {
            app = std::make_unique<BGLApp>();
            PLOGI << "Created OpenGL app.";
            break;
        }
#ifdef DIRECTX_AVAILABLE
    case DIRECTX:
        {
            app = std::make_unique<BDXApp>();
            PLOGI << "Created DirectX app.";
            break;
        }
#endif
    default:
        PLOGF << "Invalid API!";
        throw std::invalid_argument("Invalid API!");
    }

    return app.get();
}

BEApp* BasedEngine::Managers::BEAppManager::createApp(int w, int h, const std::string& title)
{
    app = std::make_unique<BVKApp>(w, h, title.c_str());
    PLOGI << "Created vulkan app.";
    return app.get();
}

BEApp* BasedEngine::Managers::BEAppManager::createApp(int w, int h, const std::string& title, const API api)
{
    switch (api)
    {
    case VULKAN:
        {
            app = std::make_unique<BVKApp>(w, h, title.c_str());
            PLOGI << "Created vulkan app.";
            break;
        }
    case OPENGL:
        {
            app = std::make_unique<BGLApp>(w, h, title.c_str());
            PLOGI << "Created OpenGL app.";
            break;
        }
#ifdef DIRECTX_AVAILABLE
    case DIRECTX:
        {
            app = std::make_unique<BDXApp>(w, h, title.c_str());
            PLOGI << "Created DirectX app.";
            break;
        }
#endif
    default:
        PLOGF << "Invalid API!";
        throw std::invalid_argument("Invalid API!");
    }

    return app.get();
}
