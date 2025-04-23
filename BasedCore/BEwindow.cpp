#include "BEwindow.h"

BEwindow::BEwindow(int w, int h, std::string title, int api) : width(w), height(h), renderAPI(api), windowName(title)
{
    initWindow();
}

BEwindow::~BEwindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void BEwindow::getCursorPos(double &xPos, double &yPos)
{
    xPos = cXPos;
    yPos = cYPos;
}

void BEwindow::createVkWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void BEwindow::switchRenderAPI(int api)
{
    renderAPI = api;
    glfwDestroyWindow(window);
    initWindow();
}

void BEwindow::setFullscreen(bool fullscreen, GLFWmonitor *monitor)
{
    wFullscreen = fullscreen;

    if (fullscreen)
    {
        const GLFWvidmode* vMode = glfwGetVideoMode(monitor);

        glfwGetWindowPos(window, &xPos, &yPos);
        glfwGetWindowSize(window, &width, &height);
        glfwSetWindowMonitor(window, monitor, xPos, yPos, vMode->width, vMode->height, 0);
    }
    else
    {
        glfwSetWindowMonitor(window, nullptr, xPos, yPos, width, height, 0);
    }
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
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, vkFrameBufferResizeCallback);

            // Setup cursor for camera rotation
            glfwSetCursorPosCallback(window, cursorPosCallback);
            setCursorMode(cursorMode);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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
            glfwSetFramebufferSizeCallback(window, glFrameBufferResizeCallback);
            break;
        }
    
        default:
            throw std::runtime_error("Invalid render API!");
    }
}

void BEwindow::vkFrameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    
    auto appWindow = reinterpret_cast<BEwindow*>(glfwGetWindowUserPointer(window));

    appWindow->frameBufferResized = true;
    appWindow->width = width;
    appWindow->height = height;
}

void BEwindow::glFrameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void BEwindow::cursorPosCallback(GLFWwindow *window, double xPos, double yPos)
{
    auto appWindow = reinterpret_cast<BEwindow*>(glfwGetWindowUserPointer(window));

    appWindow->cXPos = xPos;
    appWindow->cYPos = yPos;
}
