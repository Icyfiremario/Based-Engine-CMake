#pragma once

// STD
#include <stdexcept>

// GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class Renderer
{
    public:

        virtual void init(GLFWwindow* window) = 0;
        virtual void render() = 0;
        virtual void cleanup() = 0;

        virtual ~Renderer() = default;
};