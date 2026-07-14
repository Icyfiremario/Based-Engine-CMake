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
            break;
        }
    case BasedEngine::OPENGL:
    case BasedEngine::DIRECTX:
    default:
        throw std::invalid_argument("Invalid API!");
    }

    return app.get();
}

//BEApp* BEAppManager::createApp(int w, int h, std::string title, int api)
//{
//    return app;
//}
//
//BEApp* BEAppManager::createApp(int w, int h, std::string title)
//{
//    return app;
//}
