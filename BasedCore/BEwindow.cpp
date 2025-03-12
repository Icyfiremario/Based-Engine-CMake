#include "BEwindow.h"

BEwindow::BEwindow(int w, int h, std::string title, int api) : width(w), height(h), windowName(title), renderAPI(api)
{
    initWindow();
}

BEwindow::~BEwindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void BEwindow::switchRenderAPI(int api)
{
    renderAPI = api;
    glfwDestroyWindow(window);
}

void BEwindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    switch (renderAPI)
    {
        case BasedCore::VULKAN:
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

            break;
        }

        case BasedCore::OPENGL:
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
            glfwMakeContextCurrent(window);
            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK)
            {
                throw std::runtime_error("Failed to initialize GLEW");
            }
            glViewport(0, 0, width, height);
            break;
        }
    
        default:
            throw std::runtime_error("Invalid render API!");
    }
}

void BEwindow::frameBufferResizeCallback(GLFWwindow *window, int width, int height, int renderAPI)
{
    switch (renderAPI)
    {
        case BasedCore::VULKAN:
        {
            auto appWindow = reinterpret_cast<BEwindow*>(glfwGetWindowUserPointer(window));

            appWindow->frameBufferResized = true;
            appWindow->width = width;
            appWindow->height = height;
            break;
        }

        case BasedCore::OPENGL:
        {
            glViewport(0, 0, width, height);
            break;
        }
    
        default:
            break;
    }
}
