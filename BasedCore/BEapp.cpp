#include "BEapp.h"


BEapp::BEapp(int width, int height, int maxFrameTime, std::string name, int api) : width(width), height(height), maxFrameTime(maxFrameTime), name(name), renderAPI(api)
{
}

BEapp::~BEapp()
{
}

void BEapp::run()
{
    switch (renderAPI)
    {
        case VULKAN:
        {
            while (!appWindow.shouldClose())
            {
                glfwPollEvents();
                glfwSwapBuffers(appWindow.getWindow());
            }

            break;
        }

        case OPENGL:
            throw std::runtime_error("OpenGL is not supported yet");
    
    default:
        throw std::runtime_error("Invalid render API");
    }
    
}
