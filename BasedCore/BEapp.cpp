#include "BEapp.h"

BEapp::BEapp(int width, int height, int maxFrameTime, const std::string name, int api) : width(width), height(height), maxFrameTime(maxFrameTime), name(name), renderAPI(api)
{
    switch (renderAPI)
    {
        case BasedCore::VULKAN:
            std::make_unique<BVKDevice>(window);
            std::make_unique<BVKRenderer>(window, *VKDevice.get());
            break;

        case BasedCore::OPENGL:
            break;
    
        default:
            throw std::runtime_error("Invalid render API");
    }
}

BEapp::~BEapp()
{
}

void BEapp::run()
{
    switch (renderAPI)
    {
        case BasedCore::VULKAN:
        {
            while (!window.shouldClose())
            {
                glfwPollEvents();
            }
            break;
        }

        case BasedCore::OPENGL:
        {
            while (!window.shouldClose())
            {
                glfwPollEvents();
            }
            break;
        }
    
        default:
            throw std::runtime_error("Invalid render API");
    }
    
}
