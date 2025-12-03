#include "BEAppManager.h"

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
    case BasedEngine::OPENGL:
    case BasedEngine::DIRECTX:

    default:
        throw std::invalid_argument("Invalid API!");
    }

    return app;
}

BEApp* BEAppManager::createApp(int w, int h, std::string title)
{
    return app;
}

BEApp* BEAppManager::createApp(int w, int h, std::string title, int api)
{
    return app;
}
